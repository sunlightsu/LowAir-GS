# Demo-04: MAVLink Telemetry Viewer - 用户手册

## 1. 简介
本 Demo 展示了如何使用标准的 MAVLink 协议接收无人机的真实遥测数据，并将其与 3D 场景进行融合。程序能够实时解析 MAVLink 消息，提取无人机的位置、姿态、电池等状态信息，并在 3D 视图中驱动无人机模型运动。

## 2. 系统要求
- 操作系统：Ubuntu 20.04/22.04/24.04
- 依赖环境：Qt 5.15+ (包含 Qt3D)、MAVLink C library (v2.0)
- Python 3.8+（用于运行 MAVLink 数据模拟器）

## 3. 安装与运行
### 3.1 一键安装
在项目根目录或本目录下执行安装脚本：
```bash
./install.sh
```
*注意：MAVLink C 库的头文件已包含在项目源码中，无需额外安装。*

### 3.2 运行程序
安装完成后，在 `build` 目录下运行可执行文件：
```bash
cd build
./Demo04MavlinkTelemetryViewer
```

## 4. 操作说明
### 4.1 界面布局
- **左侧控制面板**：包含 MAVLink UDP 监听控制、状态指示灯、无人机遥测数据实时面板（经纬度、高度、姿态、电量、GPS 卫星数等）。
- **右侧 3D 视图**：显示 3D 场景、无人机模型和飞行轨迹。

### 4.2 连接 MAVLink 数据流
1. 点击左侧面板的 **Start UDP (14550)** 按钮，程序开始监听本地 14550 端口的 MAVLink UDP 数据报文。
2. 运行 MAVLink 遥测数据模拟器（或连接真实的 PX4/ArduPilot 飞控 SITL）：
   ```bash
   python3 ../../../tools/telemetry_simulator/send_mavlink_udp.py
   ```
3. 连接成功后，UDP Status 将显示为绿色，并在 Telemetry 面板中实时更新解析出的 MAVLink 数据。

### 4.3 3D 视图操作
- **旋转视角**：在 3D 视图区域按住鼠标左键并拖动。
- **缩放视角**：滚动鼠标滚轮。
- **重置视角**：点击左侧面板的 **Reset View** 按钮。
- **清除轨迹**：点击左侧面板的 **Clear Trajectory** 按钮，清空已绘制的飞行轨迹。

## 5. 常见问题
- **Q: 为什么接收不到数据？**
  - A: 默认监听端口为 14550，这是 QGroundControl (QGC) 的标准 UDP 端口。请确保没有其他地面站软件（如 QGC、Mission Planner）占用该端口，并确保数据源发送到 `127.0.0.1:14550`。
- **Q: 姿态数据显示异常？**
  - A: MAVLink 协议中的姿态角（Roll, Pitch, Yaw）通常以弧度（Radians）为单位，程序已自动将其转换为度数（Degrees）进行显示和渲染。如果仍有异常，请检查数据源发送的格式是否符合 MAVLink v1/v2 标准。
