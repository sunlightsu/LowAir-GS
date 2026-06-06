#!/usr/bin/env python3
import socket
import json
import time
import math
import argparse

def main():
    parser = argparse.ArgumentParser(description="UAV Telemetry UDP Simulator")
    parser.add_argument("--host", type=str, default="127.0.0.1", help="Target UDP Host")
    parser.add_argument("--port", type=int, default=14580, help="Target UDP Port")
    parser.add_argument("--uav_id", type=str, default="sim_01", help="UAV ID")
    parser.add_argument("--rate", type=float, default=20.0, help="Send rate in Hz")
    args = parser.parse_args()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    print(f"Starting UAV Simulator: {args.uav_id}")
    print(f"Sending to {args.host}:{args.port} at {args.rate} Hz")

    t = 0.0
    dt = 1.0 / args.rate
    
    # 初始参数
    center_x = 0.0
    center_y = 0.0
    radius_x = 30.0
    radius_y = 20.0
    base_alt = 15.0
    
    battery = 100.0

    try:
        while True:
            # 计算轨迹: 椭圆飞行
            x = center_x + radius_x * math.cos(t)
            y = center_y + radius_y * math.sin(t)
            z = base_alt + 5.0 * math.sin(t * 0.5) # 高度起伏
            
            # 计算朝向 (切线方向)
            dx = -radius_x * math.sin(t)
            dy = radius_y * math.cos(t)
            yaw = math.degrees(math.atan2(dy, dx))
            if yaw < 0:
                yaw += 360.0
                
            # 模拟轻微姿态变化
            roll = 10.0 * math.sin(t * 2.0)
            pitch = 5.0 * math.cos(t * 1.5)
            
            # 模拟电量消耗
            battery -= 0.01
            if battery < 0:
                battery = 100.0

            timestamp_ms = int(time.time() * 1000)

            state = {
                "uav_id": args.uav_id,
                "timestamp_ms": timestamp_ms,
                "frame": "SCENE",
                "x": x,
                "y": y,
                "z": z,
                "roll": roll,
                "pitch": pitch,
                "yaw": yaw,
                "battery": battery
            }

            msg = json.dumps(state).encode('utf-8')
            sock.sendto(msg, (args.host, args.port))

            t += dt * 0.5 # 控制飞行速度
            time.sleep(dt)

    except KeyboardInterrupt:
        print("\nSimulator stopped.")
    finally:
        sock.close()

if __name__ == "__main__":
    main()
