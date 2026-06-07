#pragma once
#include <QOpenGLFunctions>
#include <QVector3D>
#include <QString>
#include <vector>

/**
 * @brief ObjModelRenderer
 *
 * 使用 Assimp 加载 OBJ/PLY 等格式的摄影测量三维模型，
 * 并通过 OpenGL 立即模式渲染到场景中。
 *
 * 支持：
 *  - 多 Mesh 模型
 *  - 顶点法向量（用于光照）
 *  - 可配置的位移、缩放和颜色
 */
class ObjModelRenderer : protected QOpenGLFunctions
{
public:
    ObjModelRenderer();
    ~ObjModelRenderer();

    /**
     * @brief 加载模型文件
     * @param filePath  OBJ/PLY 文件路径
     * @param offset    模型在场景中的位移（米）
     * @param scale     缩放因子
     * @param color     渲染颜色 (r,g,b)
     * @return 是否加载成功
     */
    bool load(const QString& filePath,
              const QVector3D& offset = QVector3D(0, 0, 0),
              float scale = 1.0f,
              const QVector3D& color = QVector3D(0.7f, 0.7f, 0.7f));

    /** @brief 渲染模型（需要在 OpenGL 上下文中调用） */
    void render();

    bool isLoaded() const { return m_loaded; }
    int  triangleCount() const { return m_triangleCount; }
    int  vertexCount() const { return m_vertexCount; }
    QString modelName() const { return m_modelName; }

private:
    struct SubMesh {
        std::vector<float> vertices;   // x,y,z
        std::vector<float> normals;    // nx,ny,nz
        std::vector<unsigned int> indices;
    };

    std::vector<SubMesh> m_meshes;
    QVector3D m_offset;
    float     m_scale;
    QVector3D m_color;
    bool      m_loaded = false;
    int       m_triangleCount = 0;
    int       m_vertexCount = 0;
    QString   m_modelName;
};
