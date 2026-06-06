#pragma once
#include "MeshAsset.h"
#include <string>

// 抽象模型加载接口
class MeshLoader {
public:
    virtual ~MeshLoader() = default;
    virtual bool load(const std::string& filePath, MeshAsset& outAsset) = 0;
    virtual std::string lastError() const = 0;
};
