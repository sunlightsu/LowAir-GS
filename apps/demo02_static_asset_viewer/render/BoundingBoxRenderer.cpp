/**
 * BoundingBoxRenderer.cpp — 包围盒线框渲染器实现
 */
#include "BoundingBoxRenderer.h"

static const char *BB_VERT = R"(
#version 410 core
layout(location = 0) in vec3 aPos;
uniform mat4 uMVP;
void main() { gl_Position = uMVP * vec4(aPos, 1.0); }
)";
static const char *BB_FRAG = R"(
#version 410 core
out vec4 FragColor;
void main() { FragColor = vec4(1.0, 0.85, 0.2, 1.0); } // 黄色
)";

void BoundingBoxRenderer::initialize()
{
    initializeOpenGLFunctions();
    buildShaders();
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
}

void BoundingBoxRenderer::buildShaders()
{
    m_shader = std::make_unique<QOpenGLShaderProgram>();
    m_shader->addShaderFromSourceCode(QOpenGLShader::Vertex, BB_VERT);
    m_shader->addShaderFromSourceCode(QOpenGLShader::Fragment, BB_FRAG);
    m_shader->link();
}

void BoundingBoxRenderer::setBox(float x0, float y0, float z0,
                                  float x1, float y1, float z1)
{
    // 8 个顶点
    float verts[] = {
        x0,y0,z0, x1,y0,z0, x1,y1,z0, x0,y1,z0,
        x0,y0,z1, x1,y0,z1, x1,y1,z1, x0,y1,z1,
    };
    // 12 条边，24 个索引
    uint16_t idx[] = {
        0,1, 1,2, 2,3, 3,0,  // 底面
        4,5, 5,6, 6,7, 7,4,  // 顶面
        0,4, 1,5, 2,6, 3,7,  // 竖边
    };
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
    m_hasBox = true;
}

void BoundingBoxRenderer::render(const QMatrix4x4 &model,
                                  const QMatrix4x4 &view,
                                  const QMatrix4x4 &projection)
{
    if (!m_hasBox || !m_shader) return;
    m_shader->bind();
    m_shader->setUniformValue("uMVP", projection * view * model);
    glBindVertexArray(m_vao);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
    m_shader->release();
}

void BoundingBoxRenderer::cleanup()
{
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
}
BoundingBoxRenderer::~BoundingBoxRenderer() {}
