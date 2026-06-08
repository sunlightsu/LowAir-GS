/**
 * AssimpModelLoader.h — 基于 Assimp 的模型加载器
 *
 * 支持格式：OBJ, PLY, GLB, GLTF, FBX, STL, DAE 等 Assimp 支持的所有格式。
 * 加载后自动计算包围盒和中心点偏移。
 */

#pragma once

#include "ModelLoader.h"

class AssimpModelLoader : public ModelLoader
{
public:
    AssimpModelLoader() = default;
    ~AssimpModelLoader() override = default;

    std::unique_ptr<ModelAsset> load(
        const std::string &filePath,
        std::string &errorMsg) override;

    bool supportsFormat(const std::string &extension) const override;

private:
    void computeBoundingBox(ModelAsset &asset);
};
