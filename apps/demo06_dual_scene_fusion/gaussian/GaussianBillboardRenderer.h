#pragma once
#include "GaussianCloud.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>

// Gaussian 渲染模式
enum class GaussianRenderMode {
    Point,   // GL_POINTS 点云模式
    Splat    // Billboard 椭圆面片模式
};

class GaussianBillboardRenderer : protected QOpenGLExtraFunctions {
public:
    GaussianBillboardRenderer();
    ~GaussianBillboardRenderer();

    void initialize();
    void upload(const GaussianCloud& cloud);
    void render(const QMatrix4x4& model,
                const QMatrix4x4& view,
                const QMatrix4x4& proj,
                GaussianRenderMode mode,
                float              opacity);
    void cleanup();

private:
    QOpenGLShaderProgram m_pointProg;
    QOpenGLShaderProgram m_splatProg;
    GLuint m_vao   = 0;
    GLuint m_vbo   = 0;
    int    m_count = 0;
    bool   m_initialized = false;
    bool   m_uploaded    = false;

    void buildShaders();
    void releaseGPU();
};
