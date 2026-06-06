# 场景模型数据说明

此目录用于存放真实摄影测量或 3DGS 导出的三维场景模型。

## 数据放置规范

为避免 GitHub 仓库体积过大，**请勿**将大型模型文件（如 `.obj`, `.glb`, `.ply`, `.splat` 等）直接提交到代码仓库。

后续如果需要接入真实摄影测量模型，请将文件放置在本地的本目录下。推荐的目录结构如下：

```text
sample_data/scene/
├── campus_model.obj      # 场景网格模型 (不提交)
├── campus_model.mtl      # 材质文件 (不提交)
├── textures/             # 纹理贴图文件夹 (不提交)
├── scene_config.json     # 场景配置文件 (可提交，包含偏移、缩放等参数)
└── README.md             # 本说明文件
```

在 Demo-01 中，已内置了两个小体积的开源摄影测量/扫描模型：
1. `model_stanford_bunny_scan.obj`：真实激光扫描重建（Stanford 3D Scanning Repository）
2. `model_photogrammetry_terrain.obj`：泊松曲面重建的摄影测量风格地形

程序启动时将通过 Assimp 自动加载这两个模型进行渲染展示。对于更大的真实场景模型，请遵循上述不提交大文件的规范。
