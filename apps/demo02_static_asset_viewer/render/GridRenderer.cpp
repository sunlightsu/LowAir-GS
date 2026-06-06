/**
 * GridRenderer.cpp — 地面网格渲染器实现
 */

#include "GridRenderer.h"
#include <vector>

static const char *GRID_VERT = R"(
#version 410 core
layout(location = 0) in vec3 aPos;
uniform mat4 uView;
uniform mat4 uProjection;
void main() {
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}
)";

static const char *GRID_FRAG = R"(
#version 410 core
uniform vec4 uColor;
out vec4 FragColor;
void main() {
    FragColor = uColor;
}
)";

void GridRenderer::initialize(int gridSize, int step)
{
    initializeOpenGLFunctions();
    buildShaders();
    buildGeometry(gridSize, step);
}

void GridRenderer::buildShaders()
{
    m_shader = std::make_unique<QOpenGLShaderProgram>();
    m_shader->addShaderFromSourceCode(QOpenGLShader::Vertex, GRID_VERT);
    m_shader->addShaderFromSourceCode(QOpenGLShader::Fragment, GRID_FRAG);
    m_shader->link();
}

void GridRenderer::buildGeometry(int gridSize, int step)
{
    std::vector<float> mainVerts, majorVerts;
    float half = static_cast<float>(gridSize);

    for (int i = -gridSize; i <= gridSize; i += step) {
        float f = static_cast<float>(i);
        bool isMajor = (i % (step * 2) == 0);
        auto &target = isMajor ? majorVerts : mainVerts;
        // X 方向线
        target.insert(target.end(), {f, -half, 0.0f, f, half, 0.0f});
        // Y 方向线
        target.insert(target.end(), {-half, f, 0.0f, half, f, 0.0f});
    }

    auto upload = [&](GLuint &vao, GLuint &vbo, const std::vector<float> &data) {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glBindVertexArray(0);
    };

    upload(m_vaoMain,  m_vboMain,  mainVerts);
    upload(m_vaoMajor, m_vboMajor, majorVerts);
    m_mainCount  = static_cast<int>(mainVerts.size()  / 3);
    m_majorCount = static_cast<int>(majorVerts.size() / 3);
}

void GridRenderer::render(const QMatrix4x4 &view, const QMatrix4x4 &projection)
{
    if (!m_shader) return;
    m_shader->bind();
    m_shader->setUniformValue("uView",       view);
    m_shader->setUniformValue("uProjection", projection);

    // 次网格线：深灰色
    m_shader->setUniformValue("uColor", QVector4D(0.30f, 0.30f, 0.35f, 1.0f));
    glBindVertexArray(m_vaoMain);
    glDrawArrays(GL_LINES, 0, m_mainCount);

    // 主网格线：中灰色
    m_shader->setUniformValue("uColor", QVector4D(0.45f, 0.45f, 0.50f, 1.0f));
    glBindVertexArray(m_vaoMajor);
    glDrawArrays(GL_LINES, 0, m_majorCount);

    glBindVertexArray(0);
    m_shader->release();
}

void GridRenderer::cleanup()
{
    if (m_vaoMain)  glDeleteVertexArrays(1, &m_vaoMain);
    if (m_vboMain)  glDeleteBuffers(1, &m_vboMain);
    if (m_vaoMajor) glDeleteVertexArrays(1, &m_vaoMajor);
    if (m_vboMajor) glDeleteBuffers(1, &m_vboMajor);
}

GridRenderer::~GridRenderer() {}
