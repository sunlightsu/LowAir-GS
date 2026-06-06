/**
 * MeshRenderer.cpp — 网格渲染器实现
 *
 * 光照模型：Blinn-Phong
 *   L_ambient  = Ka * Ia
 *   L_diffuse  = Kd * max(dot(N, L), 0) * Id
 *   L_specular = Ks * pow(max(dot(N, H), 0), Ns) * Is
 *   L_total    = L_ambient + L_diffuse + L_specular
 *
 * 其中 H = normalize(L + V) 为半程向量，N 为法向量，
 * L 为光源方向，V 为视线方向。
 */

#include "MeshRenderer.h"
#include <QDebug>

// ─── GLSL 着色器源码 ─────────────────────────────────────────────────────────

static const char *VERT_SRC = R"(
#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormalMatrix;

out vec3 vFragPos;
out vec3 vNormal;
out vec3 vColor;

void main() {
    vec4 worldPos = uModel * vec4(aPosition, 1.0);
    vFragPos  = worldPos.xyz;
    vNormal   = normalize(uNormalMatrix * aNormal);
    vColor    = aColor;
    gl_Position = uProjection * uView * worldPos;
}
)";

static const char *FRAG_SRC = R"(
#version 410 core

in vec3 vFragPos;
in vec3 vNormal;
in vec3 vColor;

uniform vec3  uLightPos;    // 光源位置（世界坐标）
uniform vec3  uViewPos;     // 相机位置（世界坐标）
uniform vec3  uMatAmbient;
uniform vec3  uMatDiffuse;
uniform vec3  uMatSpecular;
uniform float uMatShininess;
uniform bool  uUseVertexColor;

out vec4 FragColor;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(uLightPos - vFragPos);
    vec3 V = normalize(uViewPos  - vFragPos);
    vec3 H = normalize(L + V);

    // 基础颜色：材质漫反射或顶点颜色
    vec3 baseColor = uUseVertexColor ? vColor : uMatDiffuse;

    // Blinn-Phong 光照
    vec3 ambient  = uMatAmbient * 0.4;
    vec3 diffuse  = baseColor * max(dot(N, L), 0.0);
    vec3 specular = uMatSpecular * pow(max(dot(N, H), 0.0), uMatShininess);

    vec3 color = ambient + diffuse + specular;

    // 背面补光（防止背面全黑）
    float backLight = max(dot(-N, L), 0.0) * 0.15;
    color += baseColor * backLight;

    FragColor = vec4(color, 1.0);
}
)";

// ─── 初始化 ──────────────────────────────────────────────────────────────────

void MeshRenderer::initialize()
{
    initializeOpenGLFunctions();
    buildShaders();
}

void MeshRenderer::buildShaders()
{
    m_shader = std::make_unique<QOpenGLShaderProgram>();
    if (!m_shader->addShaderFromSourceCode(QOpenGLShader::Vertex, VERT_SRC)) {
        qWarning() << "MeshRenderer: vertex shader error:" << m_shader->log();
        return;
    }
    if (!m_shader->addShaderFromSourceCode(QOpenGLShader::Fragment, FRAG_SRC)) {
        qWarning() << "MeshRenderer: fragment shader error:" << m_shader->log();
        return;
    }
    if (!m_shader->link()) {
        qWarning() << "MeshRenderer: shader link error:" << m_shader->log();
        return;
    }
}

// ─── 上传模型到 GPU ──────────────────────────────────────────────────────────

void MeshRenderer::uploadModel(const ModelAsset &asset)
{
    cleanup();
    if (!m_shader) return;

    m_materials = asset.materials;

    for (const auto &sub : asset.meshes) {
        if (sub.vertices.empty() || sub.indices.empty()) continue;

        GpuMesh gm;
        gm.materialIndex = sub.materialIndex;
        gm.indexCount    = static_cast<uint32_t>(sub.indices.size());

        glGenVertexArrays(1, &gm.vao);
        glGenBuffers(1, &gm.vbo);
        glGenBuffers(1, &gm.ebo);

        glBindVertexArray(gm.vao);

        // VBO：顶点数据（interleaved）
        glBindBuffer(GL_ARRAY_BUFFER, gm.vbo);
        glBufferData(GL_ARRAY_BUFFER,
                     sub.vertices.size() * sizeof(Vertex),
                     sub.vertices.data(),
                     GL_STATIC_DRAW);

        // EBO：索引数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gm.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sub.indices.size() * sizeof(uint32_t),
                     sub.indices.data(),
                     GL_STATIC_DRAW);

        // 顶点属性绑定
        // layout(location=0): position (3 floats)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, position));
        // layout(location=1): normal (3 floats)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, normal));
        // layout(location=2): texCoord (2 floats)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, texCoord));
        // layout(location=3): color (3 floats)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offsetof(Vertex, color));

        glBindVertexArray(0);
        m_gpuMeshes.push_back(gm);
    }

    m_ready = true;
}

// ─── 渲染 ────────────────────────────────────────────────────────────────────

void MeshRenderer::render(const QMatrix4x4 &model,
                          const QMatrix4x4 &view,
                          const QMatrix4x4 &projection,
                          bool wireframe)
{
    if (!m_ready || !m_shader) return;

    m_shader->bind();

    // MVP 矩阵
    m_shader->setUniformValue("uModel",      model);
    m_shader->setUniformValue("uView",       view);
    m_shader->setUniformValue("uProjection", projection);

    // 法向量矩阵 = transpose(inverse(model)) 的左上 3x3
    QMatrix4x4 normalMat = model.inverted().transposed();
    QMatrix3x3 nm3;
    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            nm3(r, c) = normalMat(r, c);
    m_shader->setUniformValue("uNormalMatrix", nm3);

    // 光源：固定在相机上方偏前方（世界坐标）
    m_shader->setUniformValue("uLightPos", QVector3D(50.0f, 80.0f, 100.0f));

    // 相机位置（从 view 矩阵逆推）
    QMatrix4x4 viewInv = view.inverted();
    m_shader->setUniformValue("uViewPos",
        QVector3D(viewInv(0,3), viewInv(1,3), viewInv(2,3)));

    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (const auto &gm : m_gpuMeshes) {
        // 设置材质
        if (gm.materialIndex >= 0 && gm.materialIndex < (int)m_materials.size()) {
            setMaterial(m_materials[gm.materialIndex]);
            m_shader->setUniformValue("uUseVertexColor", false);
        } else {
            // 无材质：使用顶点颜色
            m_shader->setUniformValue("uMatAmbient",   QVector3D(0.2f, 0.2f, 0.2f));
            m_shader->setUniformValue("uMatDiffuse",   QVector3D(0.7f, 0.7f, 0.7f));
            m_shader->setUniformValue("uMatSpecular",  QVector3D(0.1f, 0.1f, 0.1f));
            m_shader->setUniformValue("uMatShininess", 32.0f);
            m_shader->setUniformValue("uUseVertexColor", true);
        }

        glBindVertexArray(gm.vao);
        glDrawElements(GL_TRIANGLES, gm.indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_shader->release();
}

void MeshRenderer::setMaterial(const Material &mat)
{
    m_shader->setUniformValue("uMatAmbient",
        QVector3D(mat.ambient[0], mat.ambient[1], mat.ambient[2]));
    m_shader->setUniformValue("uMatDiffuse",
        QVector3D(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]));
    m_shader->setUniformValue("uMatSpecular",
        QVector3D(mat.specular[0], mat.specular[1], mat.specular[2]));
    m_shader->setUniformValue("uMatShininess", mat.shininess);
    m_shader->setUniformValue("uUseVertexColor", false);
}

// ─── 清理 ────────────────────────────────────────────────────────────────────

void MeshRenderer::cleanup()
{
    for (auto &gm : m_gpuMeshes) {
        if (gm.vao) glDeleteVertexArrays(1, &gm.vao);
        if (gm.vbo) glDeleteBuffers(1, &gm.vbo);
        if (gm.ebo) glDeleteBuffers(1, &gm.ebo);
    }
    m_gpuMeshes.clear();
    m_ready = false;
}

MeshRenderer::~MeshRenderer()
{
    // cleanup() 需要在有效 GL 上下文中调用，由 RenderWidget 负责
}
