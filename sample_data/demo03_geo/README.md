# Demo-03 地理参考数据目录说明

本目录（`sample_data/demo03_geo/`）用于存放 Demo-03 坐标转换演示程序所需的地理参考数据或预录制测试轨迹文件。

---

## 1. 默认地理原点说明

Demo-03 的默认地理参考原点由配置文件 `apps/demo03_geo_coordinate_alignment/config/demo03_geo_config.json` 指定，默认值如下：

| 参数 | 值 | 说明 |
|---|---|---|
| `origin_lat` | 30.700000 | 纬度（度，WGS84） |
| `origin_lon` | 104.000000 | 经度（度，WGS84） |
| `origin_alt` | 500.0 | 椭球高（米） |
| `udp_port` | 14581 | UDP 监听端口 |
| `scene_scale` | 1.0 | 场景缩放因子 |

---

## 2. WGS84 JSON 遥测数据格式

本目录可存放预录制的 WGS84 遥测轨迹文件（建议格式：`.jsonl`，每行一条 JSON 记录）。

每条 JSON 记录需包含以下字段：

```json
{
  "uav_id": "UAV-01",
  "timestamp_ms": 1717632000000,
  "frame": "WGS84",
  "lat": 30.700180,
  "lon": 104.000090,
  "alt": 520.0,
  "roll": 0.0,
  "pitch": 0.0,
  "yaw": 45.0,
  "battery": 98.5
}
```

| 字段 | 类型 | 说明 |
|---|---|---|
| `uav_id` | string | 无人机标识符 |
| `timestamp_ms` | number | Unix 时间戳（毫秒） |
| `frame` | string | 坐标系标识，必须为 `"WGS84"` |
| `lat` | number | 纬度（度） |
| `lon` | number | 经度（度） |
| `alt` | number | 椭球高（米） |
| `roll` | number | 横滚角（度） |
| `pitch` | number | 俯仰角（度） |
| `yaw` | number | 偏航角（度） |
| `battery` | number | 电量百分比 |

---

## 3. 预留文件说明

| 文件名 | 说明 |
|---|---|
| `sample_wgs84_track.jsonl` | 预录制的 WGS84 轨迹文件（待补充） |

---

## 4. 真实数据不提交原则

本目录**不应提交**包含真实飞行器位置信息的数据文件，以保护飞行隐私和安全。测试数据应使用 Python 模拟器生成，或使用经脱敏处理的公开数据集。

---

## 5. 与 Demo-04 的关系

Demo-03 的数据来源为 Python 模拟器（`tools/geo_telemetry_simulator/send_wgs84_uav_udp.py`），主要验证坐标转换算法的正确性。Demo-04 将在此基础上接入真实无人机或仿真平台（如 PX4 SITL / MAVLink / ROS2），届时本目录可存放对应的测试轨迹或配置文件。
