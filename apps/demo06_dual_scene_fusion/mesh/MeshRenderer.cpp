#include "MeshRenderer.h"
#include <QOpenGLContext>

MeshRenderer::MeshRenderer() {}

MeshRenderer::~MeshRenderer() {
    cleanup();
}

void MeshRenderer::buildShader() {
    const char* vsrc = R"(
        #version 330 core
        layout(location=0) in vec3 aPos;
        layout(location=1) in vec3 aNormal;
        layout(location=2) in vec3 aColor;
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;
        out vec3 vNormal;
        out vec3 vColor;
        out vec3 vFragPos;
        void main() {
            vec4 worldPos = uModel * vec4(aPos, 1.0);
            vFragPos = worldPos.xyz;
            vNormal  = mat3(transpose(inverse(uModel))) * aNormal;
            vColor   = aColor;
            gl_Position = uProj * uView * worldPos;
        }
    )";
    const char* fsrc = R"(
        #version 330 core
        in vec3 vNormal;
        in vec3 vColor;
        in vec3 vFragPos;
        uniform vec3  uDiffuseColor;
        uniform float uOpacity;
        uniform bool  uUseVertexColor;
        out vec4 fragColor;
        void main() {
            vec3 lightDir = normalize(vec3(0.5, 1.0, 0.8));
            vec3 norm     = normalize(vNormal);
            float diff    = max(dot(norm, lightDir), 0.0);
            float ambient = 0.35;
            vec3 baseCol  = uUseVertexColor ? vColor : uDiffuseColor;
            vec3 result   = (ambient + diff * 0.65) * baseCol;
            fragColor = vec4(result, uOpacity);
        }
    )";
    m_prog.addShaderFromSourceCode(QOpenGLShader::Vertex,   vsrc);
    m_prog.addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
    m_prog.link();
}

void MeshRenderer::initialize() {
    if (m_initialized) return;
    initializeOpenGLFunctions();
    buildShader();
    m_initialized = true;
}

void MeshRenderer::upload(const MeshAsset& asset) {
    if (!m_initialized) return;
    releaseGPU();
    m_gpuMeshes.clear();
    m_diffuseColors.clear();

    for (const auto& sub : asset.subMeshes) {
        SubMeshGPU g;
        glGenVertexArrays(1, &g.vao);
        glGenBuffers(1, &g.vbo);
        glGenBuffers(1, &g.ebo);
        glBindVertexArray(g.vao);

        // 构建交错 VBO: pos(3) + normal(3) + color(3) = 9 floats
        std::vector<float> buf;
        buf.reserve(sub.vertices.size() * 9);
        for (const auto& v : sub.vertices) {
            buf.push_back(v.position.x()); buf.push_back(v.position.y()); buf.push_back(v.position.z());
            buf.push_back(v.normal.x());   buf.push_back(v.normal.y());   buf.push_back(v.normal.z());
            buf.push_back(v.color.x());    buf.push_back(v.color.y());    buf.push_back(v.color.z());
        }
        glBindBuffer(GL_ARRAY_BUFFER, g.vbo);
        glBufferData(GL_ARRAY_BUFFER, buf.size() * sizeof(float), buf.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sub.indices.size() * sizeof(unsigned int),
                     sub.indices.data(), GL_STATIC_DRAW);

        GLsizei stride = 9 * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

        glBindVertexArray(0);
        g.indexCount = static_cast<int>(sub.indices.size());
        m_gpuMeshes.push_back(g);
        m_diffuseColors.push_back(sub.diffuseColor);
    }
    m_uploaded = true;
}

void MeshRenderer::render(const QMatrix4x4& model,
                          const QMatrix4x4& view,
                          const QMatrix4x4& proj,
                          MeshRenderMode    mode,
                          float             opacity) {
    if (!m_initialized || !m_uploaded) return;
    m_prog.bind();
    m_prog.setUniformValue("uModel", model);
    m_prog.setUniformValue("uView",  view);
    m_prog.setUniformValue("uProj",  proj);
    m_prog.setUniformValue("uOpacity", opacity);
    m_prog.setUniformValue("uUseVertexColor", false);

    if (mode == MeshRenderMode::Wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (mode == MeshRenderMode::Transparent || opacity < 1.0f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    }

    for (int i = 0; i < (int)m_gpuMeshes.size(); ++i) {
        m_prog.setUniformValue("uDiffuseColor", m_diffuseColors[i]);
        glBindVertexArray(m_gpuMeshes[i].vao);
        glDrawElements(GL_TRIANGLES, m_gpuMeshes[i].indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    if (mode == MeshRenderMode::Transparent || opacity < 1.0f) {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_prog.release();
}

void MeshRenderer::releaseGPU() {
    for (auto& g : m_gpuMeshes) {
        if (g.vao) glDeleteVertexArrays(1, &g.vao);
        if (g.vbo) glDeleteBuffers(1, &g.vbo);
        if (g.ebo) glDeleteBuffers(1, &g.ebo);
    }
    m_gpuMeshes.clear();
    m_uploaded = false;
}

void MeshRenderer::cleanup() {
    if (m_initialized) {
        releaseGPU();
        m_initialized = false;
    }
}
