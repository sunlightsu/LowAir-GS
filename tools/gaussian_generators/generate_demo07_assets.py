#!/usr/bin/env python3
"""
Demo-07 示例数据生成脚本
生成任务配置、虚拟目标、触发区域、轨迹回放数据等 7 个示例文件
教学演示用途，不含真实地理坐标或飞行数据
"""
import json
import math
import os
import random

OUT = "sample_data/demo07_game"
os.makedirs(OUT, exist_ok=True)

random.seed(42)

# ─── 1. 虚拟目标列表（inspection_point 类型）───────────────────────────────
targets = []
positions = [
    [3.0,  0.5,  3.0],
    [-3.0, 0.5,  3.0],
    [0.0,  0.5, -4.0],
    [4.0,  0.5, -2.0],
    [-4.0, 0.5, -2.0],
]
for i, pos in enumerate(positions):
    targets.append({
        "id": i + 1,
        "name": f"Target-{i+1:02d}",
        "type": "inspection_point",
        "position": pos,
        "radius": 1.2,
        "score": 20,
        "effect_type": "glow"
    })

with open(f"{OUT}/virtual_targets.json", "w") as f:
    json.dump({"targets": targets}, f, indent=2)
print(f"✓ virtual_targets.json  ({len(targets)} targets)")

# ─── 2. 触发区域列表（inspection_zone 类型）──────────────────────────────
zones = [
    {"id": 1, "name": "Zone-A", "type": "inspection_zone",
     "center": [0.0, 1.0, 0.0], "size": [4.0, 2.0, 4.0],
     "score": 15, "effect_type": "pulse"},
    {"id": 2, "name": "Zone-B", "type": "inspection_zone",
     "center": [5.0, 1.0, 5.0], "size": [3.0, 2.0, 3.0],
     "score": 15, "effect_type": "pulse"},
]

with open(f"{OUT}/trigger_zones.json", "w") as f:
    json.dump({"zones": zones}, f, indent=2)
print(f"✓ trigger_zones.json    ({len(zones)} zones)")

# ─── 3. 评分规则 ─────────────────────────────────────────────────────────
score_rules = {
    "max_score": 100,
    "rules": {
        "target_reached":   20,
        "zone_coverage":    30,
        "path_efficiency":  20,
        "time_efficiency":  15,
        "energy_estimate":  15
    }
}
with open(f"{OUT}/score_rules.json", "w") as f:
    json.dump(score_rules, f, indent=2)
print("✓ score_rules.json")

# ─── 4. 轨迹回放数据（JSONL 格式）────────────────────────────────────────
# 椭圆轨迹，途经所有目标点附近，飞行约 60 秒
frames = []
total_time = 60.0
fps = 10
n_frames = int(total_time * fps)

# 路径点：起点 → 各目标 → 区域中心 → 返回
waypoints = [
    [0.0, 2.0, 0.0],    # 起点（悬停）
    [3.0, 2.0, 3.0],    # Target-1
    [-3.0, 2.0, 3.0],   # Target-2
    [0.0, 2.0, -4.0],   # Target-3
    [4.0, 2.0, -2.0],   # Target-4
    [-4.0, 2.0, -2.0],  # Target-5
    [0.0, 1.5, 0.0],    # Zone-A 中心
    [5.0, 1.5, 5.0],    # Zone-B 中心
    [0.0, 2.0, 0.0],    # 返回起点
]

def lerp(a, b, t):
    return [a[i] + (b[i] - a[i]) * t for i in range(3)]

def lerp_path(waypoints, t_norm):
    n = len(waypoints) - 1
    seg = min(int(t_norm * n), n - 1)
    t_seg = (t_norm * n) - seg
    return lerp(waypoints[seg], waypoints[seg + 1], t_seg)

for i in range(n_frames):
    t = i / (n_frames - 1)
    ts = t * total_time
    pos = lerp_path(waypoints, t)
    # 加少量随机抖动
    pos[0] += random.gauss(0, 0.05)
    pos[1] += random.gauss(0, 0.02)
    pos[2] += random.gauss(0, 0.05)
    # 计算航向角（朝向下一帧）
    if i < n_frames - 1:
        next_pos = lerp_path(waypoints, (i + 1) / (n_frames - 1))
        dx = next_pos[0] - pos[0]
        dz = next_pos[2] - pos[2]
        yaw = math.degrees(math.atan2(dx, dz))
    else:
        yaw = 0.0
    battery = max(10.0, 100.0 - t * 35.0)
    frames.append({
        "t": round(ts, 2),
        "pos": [round(p, 3) for p in pos],
        "yaw": round(yaw, 1),
        "pitch": 0.0,
        "roll": 0.0,
        "battery": round(battery, 1)
    })

track_path = f"{OUT}/replay_track.jsonl"
with open(track_path, "w") as f:
    for fr in frames:
        f.write(json.dumps(fr) + "\n")
print(f"✓ replay_track.jsonl    ({len(frames)} frames, {total_time:.0f}s)")

# ─── 5. 任务配置主文件（mission_config.json）─────────────────────────────
mission_config = {
    "mission_id": "demo07_inspection_mission_01",
    "scenario_name": "Demo-07 虚拟巡检评估任务",
    "scene": {
        "mesh_path": "",
        "gaussian_path": ""
    },
    "replay": {
        "track_path": f"{OUT}/replay_track.jsonl",
        "speed": 1.0,
        "loop": False
    },
    "scoring": {
        "max_score": 100,
        "rules": {
            "target_reached":   20,
            "zone_coverage":    30,
            "path_efficiency":  20,
            "time_efficiency":  15,
            "energy_estimate":  15
        }
    },
    "targets": [
        {
            "id": t["id"],
            "name": t["name"],
            "type": t["type"],
            "position": t["position"],
            "radius": t["radius"],
            "score": t["score"],
            "effect_type": t["effect_type"]
        }
        for t in targets
    ] + [
        {
            "id": 100 + z["id"],
            "name": z["name"],
            "type": z["type"],
            "center": z["center"],
            "size": z["size"],
            "score": z["score"],
            "effect_type": z["effect_type"]
        }
        for z in zones
    ]
}

with open(f"{OUT}/mission_config.json", "w") as f:
    json.dump(mission_config, f, indent=2, ensure_ascii=False)
print("✓ mission_config.json")

# ─── 6. 事件记录示例（event_records.json）────────────────────────────────
event_records = {
    "description": "示例事件记录（教学演示用，非真实飞行数据）",
    "events": [
        {"id": 1, "type": "target_reached", "target_id": 1, "timestamp": 7.2,
         "score_delta": 20, "message": "到达目标 Target-01"},
        {"id": 2, "type": "target_reached", "target_id": 2, "timestamp": 14.5,
         "score_delta": 20, "message": "到达目标 Target-02"},
        {"id": 3, "type": "zone_entered",   "target_id": 1, "timestamp": 32.1,
         "score_delta": 15, "message": "进入区域 Zone-A"},
        {"id": 4, "type": "zone_entered",   "target_id": 2, "timestamp": 44.8,
         "score_delta": 15, "message": "进入区域 Zone-B"},
        {"id": 5, "type": "mission_complete","target_id": 0, "timestamp": 60.0,
         "score_delta": 0,  "message": "任务完成"}
    ]
}
with open(f"{OUT}/event_records.json", "w") as f:
    json.dump(event_records, f, indent=2, ensure_ascii=False)
print("✓ event_records.json")

# ─── 7. 任务报告示例（mission_report.json）───────────────────────────────
mission_report = {
    "mission_id": "demo07_inspection_mission_01",
    "scenario_name": "Demo-07 虚拟巡检评估任务",
    "total_score": 87,
    "max_score": 100,
    "targets_completed": 5,
    "total_targets": 5,
    "zones_completed": 2,
    "total_zones": 2,
    "events_triggered": 5,
    "elapsed_sec": 60.0,
    "battery_remaining": 65.0,
    "score_breakdown": {
        "target_completion": 20,
        "zone_coverage": 30,
        "path_efficiency": 17,
        "time_efficiency": 12,
        "energy_estimate": 8
    },
    "note": "教学模拟评估报告，不是真实飞行评估结果"
}
with open(f"{OUT}/mission_report.json", "w") as f:
    json.dump(mission_report, f, indent=2, ensure_ascii=False)
print("✓ mission_report.json")

print(f"\n所有示例数据已生成至 {OUT}/")
print("  1. virtual_targets.json   — 5 个虚拟目标")
print("  2. trigger_zones.json     — 2 个触发区域")
print("  3. score_rules.json       — 评分规则")
print("  4. replay_track.jsonl     — 600 帧轨迹回放数据")
print("  5. mission_config.json    — 任务配置主文件")
print("  6. event_records.json     — 示例事件记录")
print("  7. mission_report.json    — 示例任务报告")
