#include "AssimpMeshLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>

bool AssimpMeshLoader::load(const std::string& filePath, MeshAsset& outAsset) {
    outAsset.clear();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        filePath,
        aiProcess_Triangulate |
        aiProcess_GenNormals  |
        aiProcess_FlipUVs     |
        aiProcess_JoinIdenticalVertices
    );
    if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)) {
        m_lastError = importer.GetErrorString();
        return false;
    }

    outAsset.filePath = filePath;

    // 遍历所有子网格
    for (unsigned int mi = 0; mi < scene->mNumMeshes; ++mi) {
        aiMesh* aimesh = scene->mMeshes[mi];
        SubMesh sub;

        // 顶点
        for (unsigned int vi = 0; vi < aimesh->mNumVertices; ++vi) {
            MeshVertex v;
            v.position = {aimesh->mVertices[vi].x,
                          aimesh->mVertices[vi].y,
                          aimesh->mVertices[vi].z};
            if (aimesh->HasNormals()) {
                v.normal = {aimesh->mNormals[vi].x,
                            aimesh->mNormals[vi].y,
                            aimesh->mNormals[vi].z};
            }
            if (aimesh->HasTextureCoords(0)) {
                v.texCoord = {aimesh->mTextureCoords[0][vi].x,
                              aimesh->mTextureCoords[0][vi].y};
            }
            if (aimesh->HasVertexColors(0)) {
                v.color = {aimesh->mColors[0][vi].r,
                           aimesh->mColors[0][vi].g,
                           aimesh->mColors[0][vi].b};
            } else {
                v.color = {0.7f, 0.7f, 0.7f};
            }
            sub.vertices.push_back(v);

            // 更新包围盒
            outAsset.bboxMin.setX(std::min(outAsset.bboxMin.x(), v.position.x()));
            outAsset.bboxMin.setY(std::min(outAsset.bboxMin.y(), v.position.y()));
            outAsset.bboxMin.setZ(std::min(outAsset.bboxMin.z(), v.position.z()));
            outAsset.bboxMax.setX(std::max(outAsset.bboxMax.x(), v.position.x()));
            outAsset.bboxMax.setY(std::max(outAsset.bboxMax.y(), v.position.y()));
            outAsset.bboxMax.setZ(std::max(outAsset.bboxMax.z(), v.position.z()));
        }

        // 索引
        for (unsigned int fi = 0; fi < aimesh->mNumFaces; ++fi) {
            const aiFace& face = aimesh->mFaces[fi];
            for (unsigned int idx = 0; idx < face.mNumIndices; ++idx) {
                sub.indices.push_back(face.mIndices[idx]);
            }
        }

        // 材质颜色
        if (aimesh->mMaterialIndex < scene->mNumMaterials) {
            aiMaterial* mat = scene->mMaterials[aimesh->mMaterialIndex];
            aiColor3D col(0.7f, 0.7f, 0.7f);
            mat->Get(AI_MATKEY_COLOR_DIFFUSE, col);
            sub.diffuseColor = {col.r, col.g, col.b};
        }

        outAsset.totalVertices  += static_cast<int>(sub.vertices.size());
        outAsset.totalTriangles += static_cast<int>(sub.indices.size()) / 3;
        outAsset.subMeshes.push_back(std::move(sub));
    }

    m_lastError.clear();
    return true;
}
