# Demo-02: 静态三维资产加载与渲染

本演示单元展示了如何在 Qt 框架中使用 Assimp 库加载和渲染外部静态三维资产（如 OBJ、PLY、GLB 格式的摄影测量模型）。它实现了一个完整的现代 OpenGL（VAO/VBO/Shader）渲染管线，支持模型信息查看、线框模式、包围盒显示以及交互式相机控制。

## 核心功能

- **外部资产加载**：基于 Assimp，支持 OBJ、PLY、GLB 等多种标准三维格式。
- **现代 OpenGL 渲染**：使用 Qt 的 `QOpenGLShaderProgram` 和 `QOpenGLBuffer`，实现高效的网格渲染（Blinn-Phong 光照）。
- **交互式相机控制**：实现 `OrbitCamera`，支持鼠标左键旋转、右键平移、滚轮缩放，以及“适配模型”功能。
- **模型信息面板**：实时解析并显示顶点数、三角面数、材质数和包围盒尺寸。
- **多种显示模式**：支持实体渲染、线框模式（Wireframe）和包围盒（BBox）渲染切换。

## 依赖要求

- Qt 6.x（Core, Gui, Widgets, OpenGLWidgets）
- Assimp 5.x 库（`libassimp-dev`）

## 编译与运行

```bash
mkdir build && cd build
cmake ..
make -j4

# 运行程序（不带参数）
./Demo02StaticAssetViewer

# 运行程序并加载指定模型
./Demo02StaticAssetViewer --model ../../../sample_data/demo02_assets/lowair_demo_buildings.obj
```

## 目录结构

- `asset/`：模型资产解析模块（Assimp 封装）。
- `camera/`：相机控制模块（OrbitCamera）。
- `render/`：OpenGL 渲染器（Mesh、Grid、Axis、BoundingBox）。
- `MainWindow` / `RenderWidget`：UI 与 OpenGL 视图核心逻辑。
