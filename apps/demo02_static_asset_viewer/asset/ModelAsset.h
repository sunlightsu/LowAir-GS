/**
 * ModelAsset.h — 三维模型资产数据结构
 *
 * 定义加载后的模型数据结构，供渲染器使用。
 * 与具体加载库（Assimp）解耦，便于后续替换。
 */

#pragma once

#include <vector>
#include <string>
#include <array>
#include <cstdint>

// ─── 顶点结构 ───────────────────────────────────────────────────────────────

struct Vertex {
    float position[3];   // 顶点坐标 (x, y, z)
    float normal[3];     // 法向量 (nx, ny, nz)
    float texCoord[2];   // 纹理坐标 (u, v)
    float color[3];      // 顶点颜色 (r, g, b)，无纹理时使用
};

// ─── 材质结构 ───────────────────────────────────────────────────────────────

struct Material {
    std::string name;
    float ambient[3]  = {0.2f, 0.2f, 0.2f};
    float diffuse[3]  = {0.6f, 0.6f, 0.6f};
    float specular[3] = {0.1f, 0.1f, 0.1f};
    float shininess   = 32.0f;
    float opacity     = 1.0f;
    std::string diffuseTexturePath;
};

// ─── 子网格结构 ─────────────────────────────────────────────────────────────

struct SubMesh {
    std::string          name;
    std::vector<Vertex>  vertices;
    std::vector<uint32_t> indices;   // 三角面索引（每 3 个一组）
    int                  materialIndex = -1;
};

// ─── 模型资产 ───────────────────────────────────────────────────────────────

struct ModelAsset {
    std::string              filePath;
    std::vector<SubMesh>     meshes;
    std::vector<Material>    materials;

    // 包围盒（世界坐标，加载后计算）
    float bboxMin[3] = { 1e9f,  1e9f,  1e9f};
    float bboxMax[3] = {-1e9f, -1e9f, -1e9f};

    // 中心点偏移（用于居中显示）
    float centerOffset[3] = {0.0f, 0.0f, 0.0f};

    // 统计信息
    uint32_t totalVertices  = 0;
    uint32_t totalTriangles = 0;

    bool isValid() const { return !meshes.empty(); }
};


