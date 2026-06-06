/**
 * GridRenderer.h — 地面网格渲染器（现代 OpenGL）
 *
 * 渲染 XY 平面上的均匀网格，用于提供空间参考。
 * 主网格线（每 10 格）颜色较亮，次网格线颜色较暗。
 */

#pragma once

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <memory>

class GridRenderer : protected QOpenGLFunctions_4_1_Core
{
public:
    GridRenderer() = default;
    ~GridRenderer();

    void initialize(int gridSize = 100, int step = 5);
    void render(const QMatrix4x4 &view, const QMatrix4x4 &projection);
    void cleanup();

private:
    void buildShaders();
    void buildGeometry(int gridSize, int step);

    std::unique_ptr<QOpenGLShaderProgram> m_shader;
    GLuint m_vaoMain  = 0, m_vboMain  = 0;
    GLuint m_vaoMajor = 0, m_vboMajor = 0;
    int    m_mainCount  = 0;
    int    m_majorCount = 0;
};
