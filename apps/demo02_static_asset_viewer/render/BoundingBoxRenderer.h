/**
 * BoundingBoxRenderer.h — 包围盒线框渲染器
 * 渲染模型的轴对齐包围盒（AABB），用于辅助查看模型范围。
 */
#pragma once
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <memory>

class BoundingBoxRenderer : protected QOpenGLFunctions_4_1_Core
{
public:
    BoundingBoxRenderer() = default;
    ~BoundingBoxRenderer();
    void initialize();
    void setBox(float minX, float minY, float minZ,
                float maxX, float maxY, float maxZ);
    void render(const QMatrix4x4 &model,
                const QMatrix4x4 &view,
                const QMatrix4x4 &projection);
    void cleanup();
private:
    void buildShaders();
    std::unique_ptr<QOpenGLShaderProgram> m_shader;
    GLuint m_vao = 0, m_vbo = 0, m_ebo = 0;
    bool m_hasBox = false;
};
