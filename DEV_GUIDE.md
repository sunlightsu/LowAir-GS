# LowAir-GS Demo-01 二次开发手册

## 1 架构概述

LowAir-GS Demo-01 采用 Qt6 和 C++17 构建，核心渲染管线基于固定管线 OpenGL 实现。系统架构主要分为四个模块：
(1) **UI 模块** (`MainWindow`)：负责整体界面布局、深色主题样式表（QSS）管理以及用户交互事件处理。
(2) **渲染模块** (`RenderWidget`, `render/`)：基于 `QOpenGLWidget`，管理 OpenGL 上下文，包含摄影测量模型加载（Assimp）、无人机模型绘制和轨迹线绘制。
(3) **遥测通信模块** (`telemetry/`)：基于 `QUdpSocket`，负责监听网络端口并异步解析 JSON 格式的无人机状态数据。
(4) **地理坐标模块** (`geo/`)：提供坐标系转换接口，目前预留了 WGS84 到局部场景坐标（SCENE）的转换框架。

## 2 核心类说明

### 2.1 RenderWidget (三维渲染核心)
继承自 `QOpenGLWidget` 和 `QOpenGLFunctions`。
*   **职责**：初始化 OpenGL 状态（深度测试、光照），处理鼠标事件以实现相机的旋转和缩放，统筹各子渲染器（场景、模型、无人机、轨迹）的绘制调用。
*   **扩展点**：如需添加新的三维元素（如航点、禁飞区），应在此类的 `paintGL()` 方法中增加相应的绘制调用。

### 2.2 ObjModelRenderer (摄影测量模型加载器)
*   **职责**：封装 Assimp 库，从本地磁盘读取 `.obj`、`.ply` 等格式的摄影测量或激光扫描网格文件。提取顶点和法向量数据，并在 `render()` 中使用 `glDrawElements` 进行高效绘制。
*   **扩展点**：目前使用固定管线和 `GL_LIGHT0` 漫反射光照。如需支持纹理贴图（Texture Mapping），需要扩展此类以解析 Assimp 材质，并使用 `QOpenGLTexture` 进行纹理绑定。

### 2.3 UdpJsonReceiver (遥测接收器)
*   **职责**：绑定指定的 UDP 端口（默认 14580），在 `readyRead` 信号触发时读取数据报，并使用 `QJsonDocument` 解析 JSON 数据。解析成功后，通过 `dataReceived` 信号将数据分发给 UI 和渲染组件。
*   **扩展点**：如需增加新的遥测字段（如 GPS 卫星数、飞行模式），需修改此类的 JSON 解析逻辑，并同步更新 `dataReceived` 信号的参数列表。

## 3 外部接口协议

系统通过 UDP 接收 JSON 格式的遥测数据。外部设备（如无人机伴飞计算机）需按以下格式发送数据包。

### 3.1 JSON 数据结构

```json
{
  "uav_id": "sim_01",
  "timestamp_ms": 1680000000123,
  "frame": "SCENE",
  "x": 30.5,
  "y": 15.2,
  "z": 10.0,
  "roll": 2.5,
  "pitch": -1.0,
  "yaw": 45.0,
  "battery": 85.5
}
```

### 3.2 字段说明

| 字段名 | 类型 | 说明 |
|---|---|---|
| `uav_id` | String | 无人机唯一标识符 |
| `timestamp_ms` | Integer | 状态生成的时间戳（毫秒） |
| `frame` | String | 坐标系，目前固定为 `SCENE`（局部笛卡尔坐标系） |
| `x`, `y`, `z` | Double | 三维空间位置坐标（单位：米） |
| `roll`, `pitch`, `yaw` | Double | 欧拉角姿态（单位：度） |
| `battery` | Double | 电池剩余电量百分比（0.0 - 100.0） |

## 4 二次开发实践指南

### 4.1 替换或添加新的三维模型

(1) 将新的 `.obj` 或 `.ply` 模型文件放入 `sample_data/scene/` 目录。
(2) 在 `RenderWidget::initializeGL()` 中，调用 `m_objRenderer->load()` 方法加载新模型。
(3) 根据模型的实际坐标范围，调整 `load()` 方法传入的位移（offset）和缩放（scale）参数，使其在场景中正确对齐。

### 4.2 实现 WGS84 坐标转换

当前系统直接使用局部笛卡尔坐标系（SCENE）。如需接入真实的 GPS 经纬度数据：
(1) 在 JSON 协议中，将 `frame` 字段定义为 `WGS84`。
(2) 修改 `telemetry/UdpJsonReceiver.cpp`，识别 `WGS84` 坐标系。
(3) 在 `geo/CoordinateManager.cpp` 中，完善 `wgs84ToScene(double lat, double lon, double alt)` 方法。建议引入 PROJ 库或实现简单的局部切平面（Local Tangent Plane, ENU）投影算法，将经纬度转换为场景内的 X/Y/Z 坐标。

### 4.3 升级至现代 OpenGL 渲染管线

Demo-01 采用固定管线（Fixed-Function Pipeline）以快速验证 MVP 功能。在未来的迭代中，为了支持更复杂的渲染效果（如 3D Gaussian Splatting 或 PBR 材质）：
(1) 将 `QOpenGLWidget` 替换为继承自 `QOpenGLFunctions_3_3_Core` 或更高版本的核心模式。
(2) 废弃 `glBegin/glEnd` 和 `glPushMatrix` 等固定管线 API。
(3) 引入 `QOpenGLShaderProgram` 编写自定义的 Vertex Shader 和 Fragment Shader。
(4) 使用 `QOpenGLBuffer` (VBO) 和 `QOpenGLVertexArrayObject` (VAO) 管理顶点数据。
