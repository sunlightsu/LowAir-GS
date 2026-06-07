# Demo-04: MAVLink Telemetry Viewer - 二次开发指南

## 1. 架构概览
Demo-04 是 Demo-01 的进阶版本，核心差异在于使用工业标准的 MAVLink 协议替代了自定义的 JSON 格式。主要模块包括：
- **MainWindow**: 主窗口，负责界面布局和事件连接。
- **RenderWidget**: 3D 渲染组件，负责场景图构建和轨迹绘制。
- **MavlinkReceiver**: MAVLink 协议解析模块，负责监听 UDP 并解析二进制 MAVLink 消息。

## 2. 核心类说明
### 2.1 MavlinkReceiver
负责网络通信和 MAVLink 协议解析。
- `startListening(quint16 port)`: 开始监听指定端口。
- `processPendingDatagrams()`: 读取 UDP 数据报，逐字节传入 MAVLink 解析器。
- `parseMessage(const mavlink_message_t &msg)`: 根据 `msg.msgid` 分发不同类型的 MAVLink 消息（如 `GLOBAL_POSITION_INT`, `ATTITUDE`, `SYS_STATUS`）。

## 3. 扩展与修改
### 3.1 解析新的 MAVLink 消息
当前程序解析了位置、姿态和系统状态。若需解析其他消息（如 VFR_HUD 速度信息、GPS_RAW_INT 详细卫星信息），请按照以下步骤：
1. 在 `MavlinkReceiver::parseMessage` 的 `switch` 语句中添加新的 `msgid` case：
```cpp
case MAVLINK_MSG_ID_VFR_HUD: {
    mavlink_vfr_hud_t vfr_hud;
    mavlink_msg_vfr_hud_decode(&msg, &vfr_hud);
    // 处理 vfr_hud.airspeed, vfr_hud.groundspeed 等
    break;
}
```
2. 在 `MavlinkReceiver.h` 中定义新的信号（如 `vfrHudReceived`）。
3. 在 `MainWindow.cpp` 中连接该信号并更新 UI。

### 3.2 MAVLink 库版本更新
本项目源码中包含了生成的 MAVLink C 头文件（位于 `third_party/mavlink/`）。如果需要支持自定义的 MAVLink 消息或更新到最新协议版本，请使用 MAVLink 生成工具（`mavgen`）重新生成 C 头文件，并替换该目录下的内容。

## 4. 编译说明
本项目使用 CMake 进行构建。编译时会自动包含 `third_party/mavlink/c_library_v2` 目录。
```cmake
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/mavlink/c_library_v2)
add_executable(Demo04MavlinkTelemetryViewer
    main.cpp
    MainWindow.cpp
    RenderWidget.cpp
    telemetry/MavlinkReceiver.cpp
)
```
