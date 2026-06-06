/**
 * AssimpModelLoader.cpp — Assimp 模型加载器实现
 *
 * 加载流程：
 *   1. Assimp::Importer 读取文件，进行三角化、法向量计算、UV 生成等后处理
 *   2. 遍历 aiScene 的所有 aiMesh，转换为 SubMesh（Vertex + Index）
 *   3. 遍历 aiMaterial，提取 Kd/Ka/Ks/Ns/d 参数
 *   4. 计算包围盒和中心点偏移
 */

#include "AssimpModelLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <algorithm>
#include <cmath>
#include <set>
#include <functional>

// ─── 支持的格式列表 ──────────────────────────────────────────────────────────

bool AssimpModelLoader::supportsFormat(const std::string &ext) const
{
    static const std::set<std::string> supported = {
        "obj", "ply", "glb", "gltf", "fbx", "stl", "dae", "3ds", "blend", "x3d"
    };
    std::string lower = ext;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return supported.count(lower) > 0;
}

// ─── 主加载函数 ──────────────────────────────────────────────────────────────

std::unique_ptr<ModelAsset> AssimpModelLoader::load(
    const std::string &filePath,
    std::string &errorMsg)
{
    Assimp::Importer importer;

    // 后处理标志：
    //   aiProcess_Triangulate      — 强制三角化（非三角面转为三角面）
    //   aiProcess_GenSmoothNormals — 若无法向量则自动生成平滑法向量
    //   aiProcess_CalcTangentSpace — 计算切线空间（为后续 PBR 预留）
    //   aiProcess_JoinIdenticalVertices — 合并重复顶点，减少 VBO 大小
    //   aiProcess_FlipUVs          — 翻转 UV V 轴（OpenGL 坐标系）
    const unsigned int flags =
        aiProcess_Triangulate          |
        aiProcess_GenSmoothNormals     |
        aiProcess_CalcTangentSpace     |
        aiProcess_JoinIdenticalVertices|
        aiProcess_FlipUVs;

    const aiScene *scene = importer.ReadFile(filePath, flags);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        errorMsg = std::string("Assimp: ") + importer.GetErrorString();
        return nullptr;
    }

    auto asset = std::make_unique<ModelAsset>();
    asset->filePath = filePath;

    // ── 材质 ──────────────────────────────────────────────────────────────
    asset->materials.resize(scene->mNumMaterials);
    int texCount = 0;
    for (unsigned int mi = 0; mi < scene->mNumMaterials; ++mi) {
        const aiMaterial *aiMat = scene->mMaterials[mi];
        Material &mat = asset->materials[mi];

        aiString name;
        aiMat->Get(AI_MATKEY_NAME, name);
        mat.name = name.C_Str();

        aiColor3D col;
        if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, col) == AI_SUCCESS)
            mat.diffuse[0] = col.r, mat.diffuse[1] = col.g, mat.diffuse[2] = col.b;
        if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, col) == AI_SUCCESS)
            mat.ambient[0] = col.r, mat.ambient[1] = col.g, mat.ambient[2] = col.b;
        if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, col) == AI_SUCCESS)
            mat.specular[0] = col.r, mat.specular[1] = col.g, mat.specular[2] = col.b;

        float shininess = 32.0f;
        aiMat->Get(AI_MATKEY_SHININESS, shininess);
        mat.shininess = shininess;

        float opacity = 1.0f;
        aiMat->Get(AI_MATKEY_OPACITY, opacity);
        mat.opacity = opacity;

        aiString texPath;
        if (aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
            mat.diffuseTexturePath = texPath.C_Str();
            ++texCount;
        }
    }

    // ── 网格（递归遍历节点树）────────────────────────────────────────────
    std::function<void(const aiNode *)> processNode = [&](const aiNode *node) {
        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            const aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
            SubMesh sub;
            sub.name = aiMesh->mName.C_Str();
            sub.materialIndex = static_cast<int>(aiMesh->mMaterialIndex);

            // 顶点
            sub.vertices.reserve(aiMesh->mNumVertices);
            for (unsigned int vi = 0; vi < aiMesh->mNumVertices; ++vi) {
                Vertex v{};
                v.position[0] = aiMesh->mVertices[vi].x;
                v.position[1] = aiMesh->mVertices[vi].y;
                v.position[2] = aiMesh->mVertices[vi].z;

                if (aiMesh->HasNormals()) {
                    v.normal[0] = aiMesh->mNormals[vi].x;
                    v.normal[1] = aiMesh->mNormals[vi].y;
                    v.normal[2] = aiMesh->mNormals[vi].z;
                } else {
                    v.normal[1] = 1.0f;  // 默认朝上
                }

                if (aiMesh->HasTextureCoords(0)) {
                    v.texCoord[0] = aiMesh->mTextureCoords[0][vi].x;
                    v.texCoord[1] = aiMesh->mTextureCoords[0][vi].y;
                }

                if (aiMesh->HasVertexColors(0)) {
                    v.color[0] = aiMesh->mColors[0][vi].r;
                    v.color[1] = aiMesh->mColors[0][vi].g;
                    v.color[2] = aiMesh->mColors[0][vi].b;
                } else {
                    // 使用材质漫反射色作为顶点颜色
                    if (sub.materialIndex >= 0 &&
                        sub.materialIndex < (int)asset->materials.size()) {
                        const auto &mat = asset->materials[sub.materialIndex];
                        v.color[0] = mat.diffuse[0];
                        v.color[1] = mat.diffuse[1];
                        v.color[2] = mat.diffuse[2];
                    } else {
                        v.color[0] = v.color[1] = v.color[2] = 0.7f;
                    }
                }
                sub.vertices.push_back(v);
            }

            // 索引
            sub.indices.reserve(aiMesh->mNumFaces * 3);
            for (unsigned int fi = 0; fi < aiMesh->mNumFaces; ++fi) {
                const aiFace &face = aiMesh->mFaces[fi];
                if (face.mNumIndices == 3) {
                    sub.indices.push_back(face.mIndices[0]);
                    sub.indices.push_back(face.mIndices[1]);
                    sub.indices.push_back(face.mIndices[2]);
                }
            }

            asset->totalVertices  += static_cast<uint32_t>(sub.vertices.size());
            asset->totalTriangles += static_cast<uint32_t>(sub.indices.size() / 3);
            asset->meshes.push_back(std::move(sub));
        }
        for (unsigned int ci = 0; ci < node->mNumChildren; ++ci)
            processNode(node->mChildren[ci]);
    };

    processNode(scene->mRootNode);

    // 记录纹理数量
    (void)texCount;  // 通过 materials 中 diffuseTexturePath 非空计数

    computeBoundingBox(*asset);
    return asset;
}

// ─── 包围盒计算 ──────────────────────────────────────────────────────────────

void AssimpModelLoader::computeBoundingBox(ModelAsset &asset)
{
    for (const auto &mesh : asset.meshes) {
        for (const auto &v : mesh.vertices) {
            for (int i = 0; i < 3; ++i) {
                asset.bboxMin[i] = std::min(asset.bboxMin[i], v.position[i]);
                asset.bboxMax[i] = std::max(asset.bboxMax[i], v.position[i]);
            }
        }
    }
    for (int i = 0; i < 3; ++i)
        asset.centerOffset[i] = -(asset.bboxMin[i] + asset.bboxMax[i]) * 0.5f;
}
