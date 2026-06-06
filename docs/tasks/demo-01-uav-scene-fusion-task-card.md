请你接手并推进 GitHub 仓库：

https://github.com/SWJTU-AI-Lab/LowAir-GS

当前目标是完成 LowAir-GS 的第一个可运行 Demo。请先阅读仓库根目录 README.md，再重点阅读：

docs/tasks/demo-01-uav-scene-fusion-task-card.md

本次任务不是做完整 3DGS，也不是做 COLMAP/ODM 重建线，而是先完成一个具有典型性、展示性、可运行性的最小 Demo：

Demo-01：摄影测量三维模型与外部无人机实时数据的虚实融合显示系统

一、任务目标

请实现一个基于 Qt C++ 的桌面端 Demo，能够完成以下功能：

启动 Qt 桌面程序；
显示一个三维场景窗口；
场景中至少包含地面网格、坐标轴、简化低空场景元素；
支持接收外部 UDP JSON 无人机实时数据；
在三维场景中实时显示无人机位置；
显示无人机朝向和飞行轨迹；
提供 Python 脚本模拟外部无人机发送实时数据；
形成一个可以演示的完整闭环。

第一版不要追求复杂，核心是把下面这个闭环跑通：

Qt 三维窗口
    ↓
显示低空三维场景
    ↓
UDP 接收无人机状态
    ↓
三维场景中显示无人机位置和轨迹
    ↓
Python 模拟器驱动无人机运动
二、重要要求
1. 第一个 Demo 要有展示性

不要只显示一个空白窗口。即使暂时没有真实摄影测量模型，也要在程序中构建一个具有低空场景特点的简化示例场景，例如：

地面网格
坐标轴
简化建筑方块
起降点标记
无人机图标
历史轨迹线
状态面板

建议第一版内置一个简单“校园/园区低空场景”的示意模型：

几栋简化建筑
一块起降区域
一条无人机飞行轨迹
一个实时运动的无人机对象

这样演示时能直观看到“无人机进入三维场景并实时运动”。

2. 不要把大型模型文件提交进仓库

第一版不要提交大体量 OBJ、GLB、PLY 或真实摄影测量模型。可以使用程序内生成的简化几何体作为默认场景。

后续如果要加入真实摄影测量模型，只在 sample_data/scene/README.md 中说明数据放置方式，不要把大文件直接提交到 GitHub。

3. 优先保证可运行

第一版优先保证：

能编译
能启动
能看到三维场景
能收到 UDP 数据
能显示无人机运动
能显示轨迹

不要一开始引入太多复杂依赖。第一阶段可以先不引入 Assimp、PCL、3DGS、COLMAP、ODM。

4. 技术路线

请采用：

Qt 6
CMake
C++17 或 C++20
QOpenGLWidget
QUdpSocket
Qt JSON
Python 3 模拟器

如果本地 Qt 版本有差异，请在文档中说明。

三、建议创建的目录结构

请在仓库中创建或完善以下结构：

LowAir-GS/
├── apps/
│   └── uav_scene_fusion_qt/
│       ├── CMakeLists.txt
│       ├── main.cpp
│       ├── MainWindow.h
│       ├── MainWindow.cpp
│       ├── RenderWidget.h
│       ├── RenderWidget.cpp
│       │
│       ├── telemetry/
│       │   ├── UavState.h
│       │   ├── UdpJsonReceiver.h
│       │   └── UdpJsonReceiver.cpp
│       │
│       ├── render/
│       │   ├── UavRenderer.h
│       │   ├── UavRenderer.cpp
│       │   ├── TrajectoryRenderer.h
│       │   └── TrajectoryRenderer.cpp
│       │
│       ├── scene/
│       │   ├── SimpleScene.h
│       │   └── SimpleScene.cpp
│       │
│       └── geo/
│           ├── CoordinateManager.h
│           └── CoordinateManager.cpp
│
├── tools/
│   └── telemetry_simulator/
│       └── send_uav_udp.py
│
├── sample_data/
│   └── scene/
│       └── README.md
│
└── docs/
    └── tasks/
        └── demo-01-uav-scene-fusion-task-card.md

如果为了简化开发，可以先把部分渲染类合并在 RenderWidget.cpp 中，但最终请尽量保持模块清晰。

四、UDP JSON 数据格式

Qt 程序监听 UDP 端口：

14580

第一版先支持 SCENE 坐标，即外部发送的坐标直接是三维场景坐标。

JSON 格式如下：

{
  "uav_id": "sim_01",
  "timestamp_ms": 1710000000123,
  "frame": "SCENE",
  "x": 50.0,
  "y": 30.0,
  "z": 20.0,
  "roll": 0.0,
  "pitch": 0.0,
  "yaw": 90.0,
  "battery": 88.0
}

Qt 程序需要解析并更新：

无人机 ID
时间戳
位置 x/y/z
姿态 roll/pitch/yaw
电量 battery

界面中至少显示：

当前无人机 ID
当前位置
当前姿态
电量
最后更新时间
UDP 接收状态
五、Python 无人机模拟器

请实现：

tools/telemetry_simulator/send_uav_udp.py

功能要求：

每 50ms 或 100ms 发送一次 UDP JSON；
模拟无人机沿圆形或椭圆轨迹飞行；
高度可轻微上下变化；
yaw 随运动方向变化；
默认发送到 127.0.0.1:14580；
支持命令行参数修改 host、port、uav_id。

基础逻辑可以参考：

x = 50 + 20 * cos(t)
y = 50 + 20 * sin(t)
z = 25 + 5 * sin(t * 0.5)
yaw = degrees(t) % 360

运行后，Qt 界面中应看到无人机实时绕圈飞行。

六、三维显示要求

Qt 三维窗口中至少显示：

1. 灰色或深色背景
2. 地面网格
3. XYZ 坐标轴
4. 几个简化建筑块
5. 起降点或目标点
6. 无人机当前位置
7. 无人机朝向箭头
8. 历史轨迹线

无人机可以先用简化模型：

小三角锥
十字形四旋翼
箭头图标
小球 + 朝向箭头

轨迹线保存最近 1000 个点即可。

七、界面功能

MainWindow 建议包含：

左侧或右侧状态面板
中间三维 OpenGL 视图
底部日志窗口
顶部菜单或按钮

至少提供以下按钮：

启动 UDP 接收
停止 UDP 接收
清空轨迹
重置视角

状态面板显示：

UDP 状态：未启动 / 监听中 / 已接收数据
无人机 ID
位置
姿态
电量
轨迹点数量
八、坐标模块要求

第一版先支持：

SCENE 坐标直接进入三维场景

但请预留 CoordinateManager，后续用于：

WGS84 → ENU → SCENE

第一版可以实现空壳或简单接口：

QVector3D sceneToScene(const QVector3D& p);
QVector3D wgs84ToScene(double lat, double lon, double alt);

其中 wgs84ToScene 可以暂时返回默认值或 TODO，但接口要预留。

九、文档要求

请新增或更新以下文档：

1. apps/uav_scene_fusion_qt/README.md

内容包括：

项目说明
依赖环境
编译方法
运行方法
UDP 数据格式
Python 模拟器使用方法
常见问题
2. sample_data/scene/README.md

说明后续如何放置真实摄影测量模型：

sample_data/scene/campus_model.obj
sample_data/scene/campus_model.mtl
sample_data/scene/textures/
sample_data/scene/scene_config.json

并说明第一版暂不提交大模型文件。

3. 更新根目录 README 的 Demo 状态

如需要，请在 README 中补充：

Demo-01 正在实现
Qt C++ MVP
UDP JSON 无人机实时接入
十、验收标准

请确保完成后满足以下验收条件：

仓库中存在 apps/uav_scene_fusion_qt/；
CMake 可以配置工程；
Qt 程序可以编译；
Qt 程序启动后能显示三维窗口；
点击“启动 UDP 接收”后监听 14580 端口；
运行 send_uav_udp.py 后，Qt 程序能收到数据；
三维窗口中无人机位置实时变化；
能显示历史轨迹线；
状态面板能显示无人机 ID、位置、姿态、电量；
文档能指导其他学生从零运行该 demo。
十一、提交要求

请按功能分步骤提交 commit，建议 commit 粒度如下：

feat: add Qt MVP project skeleton
feat: add OpenGL scene grid and simple buildings
feat: add UDP JSON telemetry receiver
feat: add UAV state panel and trajectory rendering
feat: add Python telemetry simulator
docs: add demo-01 build and run guide

完成后请创建 Pull Request，标题建议：

feat: add Demo-01 UAV scene fusion MVP

PR 描述中请写清楚：

实现了哪些功能
如何编译运行
如何启动模拟器
当前限制
后续建议
十二、当前限制和不要做的事

本次不要做：

不要接入真实 MAVLink
不要接入 ROS2
不要实现 3DGS 渲染
不要实现 COLMAP/ODM 重建
不要提交大型三维模型
不要做爆炸特效
不要做游戏化评估

这些作为后续 Demo-02 / Demo-03 扩展。

十三、最终期望效果

最终演示时应能够做到：

打开 LowAir-GS Qt 程序
    ↓
看到一个低空三维场景
    ↓
启动 UDP 接收
    ↓
运行 Python 无人机模拟器
    ↓
三维场景中的无人机实时运动
    ↓
轨迹线持续生成
    ↓
状态面板实时刷新

这个 Demo 要体现 LowAir-GS 的第一个核心价值：

把外部无人机实时状态接入三维场景，实现低空虚实融合显示。

请优先保证这个闭环稳定、清晰、可展示。