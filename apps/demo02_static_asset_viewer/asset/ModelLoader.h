/**
 * ModelLoader.h — 模型加载器抽象接口
 *
 * 定义加载器接口，使渲染器与具体加载实现（Assimp）解耦。
 * 后续可替换为其他加载器（如 tinyobjloader、draco 等）。
 */

#pragma once

#include <memory>
#include <string>
#include "ModelAsset.h"

class ModelLoader
{
public:
    virtual ~ModelLoader() = default;

    /**
     * 加载模型文件。
     * @param filePath 文件路径（UTF-8）
     * @param errorMsg 失败时输出错误信息
     * @return 成功返回有效的 ModelAsset，失败返回 nullptr
     */
    virtual std::unique_ptr<ModelAsset> load(
        const std::string &filePath,
        std::string &errorMsg) = 0;

    /**
     * 检查是否支持该文件格式。
     * @param extension 文件扩展名（小写，不含点，如 "obj"）
     */
    virtual bool supportsFormat(const std::string &extension) const = 0;
};
