/**
 * AxisRenderer.cpp — 坐标轴渲染器实现
 */
#include "AxisRenderer.h"

static const char *AXIS_VERT = R"(
#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
uniform mat4 uView;
uniform mat4 uProjection;
out vec3 vColor;
void main() {
    vColor = aColor;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
)";

static const char *AXIS_FRAG = R"(
#version 410 core
in vec3 vColor;
out vec4 FragColor;
void main() { FragColor = vec4(vColor, 1.0); }
)";

void AxisRenderer::initialize(float len)
{
    initializeOpenGLFunctions();
    m_shader = std::make_unique<QOpenGLShaderProgram>();
    m_shader->addShaderFromSourceCode(QOpenGLShader::Vertex, AXIS_VERT);
    m_shader->addShaderFromSourceCode(QOpenGLShader::Fragment, AXIS_FRAG);
    m_shader->link();

    // 6 顶点：每轴 2 个端点，每顶点 6 floats (pos + color)
    float verts[] = {
        0,0,0, 1,0,0,  len,0,0, 1,0,0,   // X 红
        0,0,0, 0,1,0,  0,len,0, 0,1,0,   // Y 绿
        0,0,0, 0,0,1,  0,0,len, 0,0,1,   // Z 蓝
    };
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glBindVertexArray(0);
}

void AxisRenderer::render(const QMatrix4x4 &view, const QMatrix4x4 &projection)
{
    if (!m_shader) return;
    m_shader->bind();
    m_shader->setUniformValue("uView", view);
    m_shader->setUniformValue("uProjection", projection);
    glLineWidth(2.5f);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
    glLineWidth(1.0f);
    m_shader->release();
}

void AxisRenderer::cleanup()
{
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
}
AxisRenderer::~AxisRenderer() {}
