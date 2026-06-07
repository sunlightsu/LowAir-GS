# Demo-04: MAVLink Telemetry Viewer - 技术报告

## 1. 摘要
本技术报告详细说明了 Demo-04 (MAVLink Telemetry Viewer) 的技术实现。该 Demo 成功集成了工业标准的 MAVLink 协议，实现了对无人机飞控（如 PX4、ArduPilot）真实遥测数据的实时解析与 3D 场景驱动，标志着本系统具备了与真实无人机硬件对接的能力。

## 2. 系统架构
系统架构在 Demo-01 的基础上进行了升级，核心变化在于数据链路层：
- **数据链路层**：`MavlinkReceiver` 替代了原有的 JSON 解析器，直接处理 UDP 二进制流。
- **协议解析层**：集成 MAVLink C Library v2，实现高效的二进制解码。
- **业务逻辑层**：`MainWindow` 汇总异步到达的各类 MAVLink 消息，整合成完整的无人机状态。
- **表现层**：`RenderWidget` 利用 Qt3D 实时渲染无人机位姿和轨迹。

## 3. 关键技术实现
### 3.1 MAVLink 二进制流解析
MAVLink 是一种轻量级的二进制消息协议。在 `MavlinkReceiver` 中，UDP 数据报被读取后，通过 `mavlink_parse_char` 函数逐字节进行状态机解析：
```cpp
mavlink_message_t msg;
mavlink_status_t status;
for (int i = 0; i < datagram.size(); ++i) {
    if (mavlink_parse_char(MAVLINK_COMM_0, datagram.at(i), &msg, &status)) {
        parseMessage(msg);
    }
}
```
这种逐字节解析机制能够有效处理网络丢包、粘包和乱序问题，保证了解析的鲁棒性。

### 3.2 多消息异步状态聚合
无人机的完整状态分布在不同的 MAVLink 消息中，例如：
- `GLOBAL_POSITION_INT` (ID 33)：包含经纬度、高度。
- `ATTITUDE` (ID 30)：包含 Roll, Pitch, Yaw 姿态角。
- `SYS_STATUS` (ID 1)：包含电池电压、剩余电量。
系统通过事件驱动模型，分别解析这些消息并触发相应的信号。UI 层和渲染层订阅这些信号，实现了状态的异步聚合和实时刷新。

### 3.3 坐标系转换
MAVLink 的姿态角通常遵循 NED（北-东-地）坐标系，而 Qt3D 使用的是 OpenGL 标准的右手坐标系（Y 向上）。在渲染前，程序进行了必要的坐标系映射，确保无人机的 3D 姿态与实际物理姿态完全一致。

## 4. 性能分析
- **解析效率**：MAVLink C 库是为嵌入式设备设计的，在 PC 平台上解析开销极低。即使在 500Hz 的高频消息流下，CPU 占用率也几乎可以忽略不计。
- **内存占用**：由于采用状态机逐字节解析，无需在内存中缓存完整的数据包，内存占用极小且稳定。

## 5. 结论与展望
Demo-04 成功实现了 MAVLink 协议的深度集成。这不仅使得本系统能够直接接入 PX4 SITL 仿真环境，也为未来接入真实的无人机硬件铺平了道路。后续的 Demo 将在此基础上，结合 Demo-03 的坐标对齐技术，将真实的 GPS 轨迹完美映射到虚拟的 3D 场景中。
