#pragma once
#include <vector>
#include <string>
#include <QVector3D>
#include <QVector2D>

// 单个顶点数据结构
struct MeshVertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texCoord;
    QVector3D color;  // 顶点颜色（若无纹理时使用）
};

// 单个子网格（对应一个 Assimp Mesh）
struct SubMesh {
    std::vector<MeshVertex>   vertices;
    std::vector<unsigned int> indices;
    QVector3D                 diffuseColor = {0.7f, 0.7f, 0.7f};
    bool                      hasTexture   = false;
};

// 整个模型资产（包含多个子网格）
struct MeshAsset {
    std::vector<SubMesh> subMeshes;
    std::string          filePath;
    int                  totalVertices  = 0;
    int                  totalTriangles = 0;

    // 包围盒
    QVector3D bboxMin = { 1e9f,  1e9f,  1e9f};
    QVector3D bboxMax = {-1e9f, -1e9f, -1e9f};
    QVector3D center() const {
        return (bboxMin + bboxMax) * 0.5f;
    }
    QVector3D size() const {
        return bboxMax - bboxMin;
    }

    bool isLoaded() const { return !subMeshes.empty(); }
    void clear() {
        subMeshes.clear();
        filePath.clear();
        totalVertices  = 0;
        totalTriangles = 0;
        bboxMin = { 1e9f,  1e9f,  1e9f};
        bboxMax = {-1e9f, -1e9f, -1e9f};
    }
};
