#pragma once
#include "MeshLoader.h"
#include <string>

// 基于 Assimp 的模型加载器，支持 OBJ / PLY / GLB / FBX 等格式
class AssimpMeshLoader : public MeshLoader {
public:
    bool load(const std::string& filePath, MeshAsset& outAsset) override;
    std::string lastError() const override { return m_lastError; }

private:
    std::string m_lastError;
};
