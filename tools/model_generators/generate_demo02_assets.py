#!/usr/bin/env python3
"""
Demo-02 示例资产生成脚本
生成用于 Demo-02 静态三维资产查看器的小型教学演示模型。

生成文件：
  - lowair_demo_buildings.obj / .mtl  (3~5 个简化建筑块，带基础材质)
  - lowair_demo_terrain.obj           (规则网格地形，带轻微高度起伏)
  - demo02_asset_config.json          (Demo-02 默认配置文件)

说明：
  这些模型是程序生成的小型教学演示模型，不是真实测绘成果，
  不代表任何真实地理坐标或真实建筑物。
"""

import math
import json
import os

OUTPUT_DIR = os.path.join(
    os.path.dirname(os.path.abspath(__file__)),
    "../../sample_data/demo02_assets"
)


# ─────────────────────────────────────────────
# 1. 建筑块模型 (OBJ + MTL)
# ─────────────────────────────────────────────

def write_mtl(path: str):
    """生成 MTL 材质文件（5 种建筑颜色）。"""
    materials = [
        ("mat_building_a", (0.55, 0.60, 0.65)),   # 蓝灰
        ("mat_building_b", (0.70, 0.65, 0.55)),   # 暖黄
        ("mat_building_c", (0.60, 0.70, 0.60)),   # 浅绿
        ("mat_building_d", (0.75, 0.55, 0.50)),   # 砖红
        ("mat_building_e", (0.65, 0.65, 0.75)),   # 淡紫灰
        ("mat_roof",       (0.35, 0.35, 0.40)),   # 深灰屋顶
        ("mat_terrain",    (0.45, 0.55, 0.40)),   # 地形绿灰
    ]
    lines = ["# LowAir-GS Demo-02 Material Library\n"]
    for name, (r, g, b) in materials:
        lines += [
            f"newmtl {name}\n",
            f"Ka {r:.3f} {g:.3f} {b:.3f}\n",
            f"Kd {r:.3f} {g:.3f} {b:.3f}\n",
            f"Ks 0.100 0.100 0.100\n",
            f"Ns 32.0\n",
            f"d 1.0\n",
            "\n",
        ]
    with open(path, "w") as f:
        f.writelines(lines)
    print(f"  Written: {path}")


def box_faces(v_offset: int):
    """
    返回一个立方体的 12 个三角面（顶点索引，1-based，相对于 v_offset）。
    立方体顶点顺序：
      0=(-x,-y,-z) 1=(+x,-y,-z) 2=(+x,+y,-z) 3=(-x,+y,-z)
      4=(-x,-y,+z) 5=(+x,-y,+z) 6=(+x,+y,+z) 7=(-x,+y,+z)
    """
    o = v_offset
    return [
        # 底面 -z
        (o+1, o+3, o+2), (o+1, o+4, o+3),
        # 顶面 +z
        (o+5, o+6, o+7), (o+5, o+7, o+8),
        # 前面 -y
        (o+1, o+2, o+6), (o+1, o+6, o+5),
        # 后面 +y
        (o+3, o+4, o+8), (o+3, o+8, o+7),
        # 左面 -x
        (o+1, o+5, o+8), (o+1, o+8, o+4),
        # 右面 +x
        (o+2, o+3, o+7), (o+2, o+7, o+6),
    ]


def add_box(vertices, cx, cy, cz, sx, sy, sz):
    """向顶点列表中添加一个 AABB 立方体的 8 个顶点。"""
    hx, hy, hz = sx / 2, sy / 2, sz / 2
    vertices += [
        (cx - hx, cy - hy, cz - hz),
        (cx + hx, cy - hy, cz - hz),
        (cx + hx, cy + hy, cz - hz),
        (cx - hx, cy + hy, cz - hz),
        (cx - hx, cy - hy, cz + hz),
        (cx + hx, cy - hy, cz + hz),
        (cx + hx, cy + hy, cz + hz),
        (cx - hx, cy + hy, cz + hz),
    ]


def write_buildings_obj(obj_path: str, mtl_name: str):
    """
    生成 5 栋简化建筑块 OBJ 模型。
    每栋建筑由「主体」+「屋顶」两个 box 组成。
    坐标系：X 东，Y 北，Z 上（局部 SCENE 坐标，单位 m）。
    """
    # (cx, cy, 主体高度, 宽度x, 宽度y, 材质)
    buildings = [
        ( 0.0,  0.0, 12.0, 8.0, 8.0, "mat_building_a"),
        (18.0,  5.0,  8.0, 6.0, 6.0, "mat_building_b"),
        (-15.0, 10.0, 16.0, 7.0, 9.0, "mat_building_c"),
        ( 8.0, -18.0, 10.0, 5.0, 7.0, "mat_building_d"),
        (-10.0,-12.0,  6.0, 9.0, 5.0, "mat_building_e"),
    ]

    vertices = []
    groups = []   # list of (group_name, mat, face_list)

    for i, (cx, cy, h, sx, sy, mat) in enumerate(buildings):
        v_off = len(vertices)
        # 主体 box: 底部 z=0，顶部 z=h
        add_box(vertices, cx, cy, h / 2, sx, sy, h)
        faces_body = box_faces(v_off)
        groups.append((f"building_{i+1:02d}_body", mat, faces_body))

        # 屋顶 box: 薄板，宽度略大，高度 1.5m
        v_off2 = len(vertices)
        add_box(vertices, cx, cy, h + 0.75, sx + 0.5, sy + 0.5, 1.5)
        faces_roof = box_faces(v_off2)
        groups.append((f"building_{i+1:02d}_roof", "mat_roof", faces_roof))

    # 统计
    n_verts = len(vertices)
    n_faces = sum(len(g[2]) for g in groups)

    lines = [
        "# LowAir-GS Demo-02 Buildings Model\n",
        "# 5 simplified building blocks for teaching demonstration.\n",
        "# NOT a real photogrammetry result.\n",
        f"# Vertices: {n_verts}  Triangles: {n_faces}\n",
        f"mtllib {mtl_name}\n",
        "\n",
    ]
    for x, y, z in vertices:
        lines.append(f"v {x:.4f} {y:.4f} {z:.4f}\n")
    lines.append("\n")

    for gname, mat, faces in groups:
        lines.append(f"g {gname}\n")
        lines.append(f"usemtl {mat}\n")
        for a, b, c in faces:
            lines.append(f"f {a} {b} {c}\n")
        lines.append("\n")

    with open(obj_path, "w") as f:
        f.writelines(lines)
    print(f"  Written: {obj_path}  ({n_verts} verts, {n_faces} tris)")
    return n_verts, n_faces


def write_terrain_obj(obj_path: str):
    """
    生成规则网格地形 OBJ（50×50 格，带轻微高度起伏）。
    范围：X ∈ [-40, 40]，Y ∈ [-40, 40]，Z ∈ [-1.5, 1.5]。
    模拟摄影测量地形网格，不代表真实地理坐标。
    """
    NX, NY = 50, 50
    X_RANGE = (-40.0, 40.0)
    Y_RANGE = (-40.0, 40.0)

    def height(x, y):
        """简单正弦叠加，模拟地形起伏。"""
        return (
            0.6 * math.sin(x * 0.15) * math.cos(y * 0.12)
            + 0.4 * math.sin(x * 0.08 + y * 0.10)
            + 0.3 * math.cos(x * 0.20 - y * 0.18)
        )

    xs = [X_RANGE[0] + (X_RANGE[1] - X_RANGE[0]) * i / NX for i in range(NX + 1)]
    ys = [Y_RANGE[0] + (Y_RANGE[1] - Y_RANGE[0]) * j / NY for j in range(NY + 1)]

    vertices = []
    for y in ys:
        for x in xs:
            vertices.append((x, y, height(x, y)))

    def idx(i, j):  # 1-based
        return j * (NX + 1) + i + 1

    faces = []
    for j in range(NY):
        for i in range(NX):
            a, b = idx(i, j), idx(i + 1, j)
            c, d = idx(i + 1, j + 1), idx(i, j + 1)
            faces.append((a, b, c))
            faces.append((a, c, d))

    n_verts = len(vertices)
    n_faces = len(faces)

    lines = [
        "# LowAir-GS Demo-02 Terrain Model\n",
        "# Regular grid terrain with sinusoidal height variation.\n",
        "# Simulates photogrammetry terrain mesh. NOT real geographic data.\n",
        f"# Vertices: {n_verts}  Triangles: {n_faces}\n",
        "mtllib lowair_demo_buildings.mtl\n",
        "\n",
        "g terrain\n",
        "usemtl mat_terrain\n",
        "\n",
    ]
    for x, y, z in vertices:
        lines.append(f"v {x:.4f} {y:.4f} {z:.4f}\n")
    lines.append("\n")
    for a, b, c in faces:
        lines.append(f"f {a} {b} {c}\n")

    with open(obj_path, "w") as f:
        f.writelines(lines)
    print(f"  Written: {obj_path}  ({n_verts} verts, {n_faces} tris)")
    return n_verts, n_faces


def write_config(path: str):
    """生成 Demo-02 默认配置文件。"""
    cfg = {
        "demo": "Demo-02",
        "description": "Static 3D Asset Viewer — teaching demonstration config",
        "default_model": "sample_data/demo02_assets/lowair_demo_buildings.obj",
        "unit": "meter",
        "coordinate_frame": "LOCAL_SCENE",
        "initial_transform": {
            "scale": 1.0,
            "translation": [0.0, 0.0, 0.0],
            "rotation_deg": [0.0, 0.0, 0.0]
        },
        "display": {
            "show_grid": True,
            "show_axis": True,
            "show_bounding_box": True,
            "wireframe": False,
            "background_color": "#1E1E2E"
        }
    }
    with open(path, "w") as f:
        json.dump(cfg, f, indent=2, ensure_ascii=False)
    print(f"  Written: {path}")


# ─────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────

def main():
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    print(f"Output directory: {OUTPUT_DIR}\n")

    mtl_path = os.path.join(OUTPUT_DIR, "lowair_demo_buildings.mtl")
    obj_b_path = os.path.join(OUTPUT_DIR, "lowair_demo_buildings.obj")
    obj_t_path = os.path.join(OUTPUT_DIR, "lowair_demo_terrain.obj")
    cfg_path = os.path.join(OUTPUT_DIR, "demo02_asset_config.json")

    write_mtl(mtl_path)
    bv, bf = write_buildings_obj(obj_b_path, "lowair_demo_buildings.mtl")
    tv, tf = write_terrain_obj(obj_t_path)
    write_config(cfg_path)

    print(f"\nSummary:")
    print(f"  Buildings: {bv} vertices, {bf} triangles")
    print(f"  Terrain  : {tv} vertices, {tf} triangles")
    print(f"\nAll assets generated successfully.")


if __name__ == "__main__":
    main()
