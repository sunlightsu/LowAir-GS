/**
 * MeshRenderer.h — 网格渲染器（现代 OpenGL VAO/VBO/EBO）
 *
 * 渲染流程：
 *   1. 初始化时将 ModelAsset 的每个 SubMesh 上传到 GPU（VAO + VBO + EBO）
 *   2. 每帧调用 render() 绑定 VAO，设置 uniform，执行 glDrawElements
 *   3. 支持实体（Solid）和线框（Wireframe）两种模式
 *
 * Shader 说明：
 *   顶点着色器：接收 position(vec3), normal(vec3), texCoord(vec2), color(vec3)
 *   片元着色器：Blinn-Phong 光照模型 + 顶点颜色
 */

#pragma once

#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <vector>
#include <memory>
#include "../asset/ModelAsset.h"

struct GpuMesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    uint32_t indexCount = 0;
    int materialIndex = -1;
};

class MeshRenderer : protected QOpenGLFunctions_4_1_Core
{
public:
    MeshRenderer() = default;
    ~MeshRenderer();

    void initialize();
    void uploadModel(const ModelAsset &asset);
    void render(const QMatrix4x4 &model,
                const QMatrix4x4 &view,
                const QMatrix4x4 &projection,
                bool wireframe = false);
    void cleanup();

    bool isReady() const { return m_ready; }

private:
    void buildShaders();
    void setMaterial(const Material &mat);

    std::unique_ptr<QOpenGLShaderProgram> m_shader;
    std::vector<GpuMesh>   m_gpuMeshes;
    std::vector<Material>  m_materials;
    bool m_ready = false;
};
