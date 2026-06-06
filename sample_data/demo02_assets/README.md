# Demo-02 示例资产

本目录包含用于 `Demo-02` 的静态三维资产文件，主要用于验证外部 OBJ 模型的加载与渲染管线。

## 资产列表

1. **`lowair_demo_buildings.obj`**
   - **描述**：5 栋不同颜色的简化建筑块，用于验证多网格（Mesh）和多材质（Material）加载。
   - **规格**：80 顶点，120 三角面，8 个材质。
   - **配套文件**：`lowair_demo_buildings.mtl`。

2. **`lowair_demo_terrain.obj`**
   - **描述**：摄影测量风格的平缓地形网格，用于验证较高面数单网格的加载性能。
   - **规格**：2601 顶点，5000 三角面，单材质。
   - **配套文件**：`lowair_demo_terrain.mtl`。

## 资产生成

这些资产由 `tools/model_generators/generate_demo02_assets.py` 脚本自动生成。如果文件丢失或需要重新生成，请在仓库根目录执行：

```bash
python3 tools/model_generators/generate_demo02_assets.py
```
