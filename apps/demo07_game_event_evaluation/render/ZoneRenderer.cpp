#include "ZoneRenderer.h"
#include <cmath>

// 顶点着色器：支持脉冲缩放动画
static const char* ZONE_VERT = R"(
#version 330 core
layout(location=0) in vec3 aPos;
uniform mat4 uMVP;
uniform vec3 uCenter;
uniform vec3 uSize;
uniform float uPulseScale;  // 脉冲缩放因子（1.0 = 无缩放）
void main(){
    vec3 scaled = aPos * uSize * 0.5 * uPulseScale;
    gl_Position = uMVP * vec4(uCenter + scaled, 1.0);
}
)";

static const char* ZONE_FRAG = R"(
#version 330 core
uniform vec4 uColor;
out vec4 fragColor;
void main(){ fragColor = uColor; }
)";

ZoneRenderer::~ZoneRenderer() { cleanup(); }

void ZoneRenderer::initialize() {
    initializeOpenGLFunctions();
    m_prog.addShaderFromSourceCode(QOpenGLShader::Vertex,   ZONE_VERT);
    m_prog.addShaderFromSourceCode(QOpenGLShader::Fragment, ZONE_FRAG);
    m_prog.link();
    buildBoxGeometry();
    m_initialized = true;
}

void ZoneRenderer::buildBoxGeometry() {
    // 单位立方体顶点（-1 到 1）
    static const float verts[] = {
        -1,-1,-1,  1,-1,-1,  1, 1,-1, -1, 1,-1,
        -1,-1, 1,  1,-1, 1,  1, 1, 1, -1, 1, 1
    };
    // 12 条棱（24 个索引）
    static const unsigned short idx[] = {
        0,1, 1,2, 2,3, 3,0,
        4,5, 5,6, 6,7, 7,4,
        0,4, 1,5, 2,6, 3,7
    };
    m_indexCount = 24;

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ibo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void ZoneRenderer::render(const QMatrix4x4& mvp, const std::vector<TriggerZone>& zones) {
    if (!m_initialized) return;

    // 更新脉冲时间
    m_pulseTime += 0.016f;  // 约 60Hz 调用时每帧 ~16ms

    m_prog.bind();
    m_prog.setUniformValue("uMVP", mvp);
    glBindVertexArray(m_vao);

    for (const auto& z : zones) {
        m_prog.setUniformValue("uCenter", z.center);
        m_prog.setUniformValue("uSize",   z.size);

        QVector4D color;
        float pulseScale = 1.0f;

        if (z.completed) {
            // 已完成：绿色，无脉冲
            color = QVector4D(0.1f, 0.9f, 0.2f, 0.7f);
            pulseScale = 1.0f;
        } else if (z.triggered) {
            // 触发中：亮蓝色，脉冲呼吸动画
            float pulse = 0.05f * std::sin(m_pulseTime * 4.0f);
            pulseScale = 1.0f + pulse;
            float brightness = 0.7f + 0.3f * std::sin(m_pulseTime * 4.0f);
            color = QVector4D(0.1f, 0.5f * brightness, 1.0f * brightness, 0.85f);
        } else {
            // 未触发：半透明蓝色
            color = QVector4D(0.2f, 0.6f, 1.0f, 0.5f);
            pulseScale = 1.0f;
        }

        m_prog.setUniformValue("uColor", color);
        m_prog.setUniformValue("uPulseScale", pulseScale);
        glDrawElements(GL_LINES, m_indexCount, GL_UNSIGNED_SHORT, nullptr);
    }

    glBindVertexArray(0);
    m_prog.release();
}

void ZoneRenderer::cleanup() {
    if (m_vao) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
    if (m_vbo) { glDeleteBuffers(1, &m_vbo);      m_vbo = 0; }
    if (m_ibo) { glDeleteBuffers(1, &m_ibo);      m_ibo = 0; }
}
