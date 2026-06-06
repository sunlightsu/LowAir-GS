/**
 * AxisRenderer.h — 坐标轴渲染器
 * X轴=红色, Y轴=绿色, Z轴=蓝色（OpenGL 右手坐标系）
 */
#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <memory>

class AxisRenderer : protected QOpenGLFunctions_4_1_Core
{
public:
    AxisRenderer() = default;
    ~AxisRenderer();
    void initialize(float length = 30.0f);
    void render(const QMatrix4x4 &view, const QMatrix4x4 &projection);
    void cleanup();
private:
    std::unique_ptr<QOpenGLShaderProgram> m_shader;
    GLuint m_vao = 0, m_vbo = 0;
};
