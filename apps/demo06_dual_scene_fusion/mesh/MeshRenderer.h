#pragma once
#include "MeshAsset.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <vector>

// Mesh 渲染模式
enum class MeshRenderMode {
    Solid,       // 实体渲染（带光照）
    Wireframe,   // 线框渲染
    Transparent  // 半透明渲染
};

// 单个子网格的 GPU 缓冲区
struct SubMeshGPU {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    int    indexCount = 0;
};

class MeshRenderer : protected QOpenGLExtraFunctions {
public:
    MeshRenderer();
    ~MeshRenderer();

    void initialize();
    void upload(const MeshAsset& asset);
    void render(const QMatrix4x4& model,
                const QMatrix4x4& view,
                const QMatrix4x4& proj,
                MeshRenderMode    mode,
                float             opacity);
    void cleanup();

private:
    QOpenGLShaderProgram m_prog;
    std::vector<SubMeshGPU> m_gpuMeshes;
    std::vector<QVector3D>  m_diffuseColors;
    bool m_initialized = false;
    bool m_uploaded    = false;

    void buildShader();
    void releaseGPU();
};
