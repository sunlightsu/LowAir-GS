# 开源工具清单与第一阶段选型说明

本文档用于汇总 LowAir-GS 第一阶段可以使用的开源工具、数据集合、算法模块和推荐组合。它的定位不是简单列链接，而是帮助学生明确：每个工具解决什么问题、什么时候用、是否必须安装、第一阶段做到什么程度即可。

---

## 1. 第一阶段推荐工具链

第一阶段建议采用如下主工具链：

```text
Git / GitHub
    ↓
Conda / Python
    ↓
Nerfstudio + Splatfacto
    ↓
COLMAP 位姿估计
    ↓
3D Gaussian Splatting 训练
    ↓
Nerfstudio Viewer / SuperSplat 可视化
    ↓
实践报告与 Pull Request 提交
```

最小可行组合：

```text
Nerfstudio Splatfacto + COLMAP + SuperSplat
```

这套组合的优势是：

- 入门门槛相对低；
- 命令行清晰，适合写实验报告；
- 可直接训练和查看 3DGS 结果；
- 便于后续扩展到自采无人机影像、语义分割和低空场景应用。

---

## 2. 工具总览

| 工具 | 类型 | 第一阶段是否必选 | 主要作用 | 推荐使用场景 |
|---|---|---|---|---|
| Git / GitHub | 协作工具 | 必选 | 代码管理、分支协作、Pull Request | 所有学生必须掌握 |
| Miniconda / Conda | 环境管理 | 必选 | 创建 Python 实验环境 | 避免污染系统环境 |
| Nerfstudio | 训练框架 | 必选 | 数据下载、训练、Viewer、导出 | 第一阶段主框架 |
| Splatfacto | 3DGS 方法 | 必选 | Nerfstudio 中的 3DGS 实现 | 跑通第一个 3DGS Demo |
| COLMAP | SfM / MVS | 推荐必选 | 相机位姿估计、稀疏点云 | 自采数据和 3DGS 初始化 |
| SuperSplat | 可视化工具 | 推荐必选 | 浏览、检查、编辑、发布 splat 模型 | 展示和答辩效果很好 |
| gsplat | 3DGS 后端库 | 自动/选用 | CUDA 高斯栅格化后端 | Nerfstudio Splatfacto 底层依赖 |
| graphdeco 3DGS | 官方参考实现 | 选用 | 原始 3DGS 论文复现 | 后续算法复现和对比 |
| OpenDroneMap / WebODM | 无人机摄影测量 | 选用 | 正射图、点云、网格、DEM | 后续无人机数据基线 |
| FFmpeg | 视频处理 | 选用 | 视频抽帧、压缩、格式转换 | 处理无人机视频数据 |
| OpenCV | 图像处理 | 选用 | 模糊检测、裁剪、缩放、图像统计 | 数据清洗与预处理 |
| CloudCompare / MeshLab | 点云与模型查看 | 选用 | 查看点云、网格、几何结果 | 传统重建结果检查 |

---

## 3. 必选工具说明

### 3.1 Git / GitHub

用途：

- 克隆 LowAir-GS 仓库；
- 创建个人分支；
- 提交实验文档、脚本和报告；
- 通过 Pull Request 完成教师审核。

学生需要掌握的最小命令：

```bash
git clone https://github.com/SWJTU-AI-Lab/LowAir-GS.git
cd LowAir-GS
git checkout -b student/<your-name>-stage1

git status
git add .
git commit -m "docs: add stage1 practice report"
git push origin student/<your-name>-stage1
```

第一阶段验收要求：

```text
能够独立提交一个文档类 Pull Request。
```

---

### 3.2 Conda / Python

用途：

- 创建隔离环境；
- 管理 Python 版本；
- 安装 Nerfstudio、OpenCV 等依赖。

推荐基础环境：

```bash
conda create -n lowair-gs python=3.10 -y
conda activate lowair-gs
python --version
pip --version
```

说明：

- 不建议学生直接在系统 Python 中安装依赖；
- 如果使用实验室服务器，建议每组一个独立 Conda 环境；
- PyTorch 与 CUDA 版本应根据服务器 GPU 驱动统一配置。

---

### 3.3 Nerfstudio

用途：

- 下载公开样例数据；
- 运行 Splatfacto 训练；
- 打开 Web Viewer；
- 导出 Gaussian Splat 模型；
- 后续扩展到自定义数据集。

常用命令：

```bash
ns-install-cli
ns-download-data --help
ns-train --help
ns-viewer --help
ns-export --help
```

第一阶段示例：

```bash
ns-download-data nerfstudio --capture-name=poster
ns-train splatfacto --data data/nerfstudio/poster
```

学生需要记录：

```text
Nerfstudio 版本：
训练命令：
训练数据：
输出目录：
Viewer 地址：
训练耗时：
显存占用：
```

---

### 3.4 Splatfacto

Splatfacto 是 Nerfstudio 中的 3D Gaussian Splatting 方法实现。第一阶段推荐使用它，而不是直接使用原始 3DGS 代码，原因是 Nerfstudio 已经封装了数据处理、训练、Viewer 和导出流程，更适合教学实践。

简化理解：

```text
COLMAP / 样例数据提供相机位姿和初始点云
        ↓
Splatfacto 初始化 3D Gaussians
        ↓
根据相机视角渲染图像
        ↓
与真实图像比较误差
        ↓
优化高斯的位置、尺度、旋转、颜色和透明度
        ↓
得到可实时浏览的 3DGS 场景
```

第一阶段只要求：

```text
能够用 Splatfacto 跑通一个公开样例数据，并保存截图和日志。
```

---

### 3.5 COLMAP

用途：

- 从多张图像中估计相机位姿；
- 生成稀疏点云；
- 为 3DGS 提供几何初始化。

COLMAP 在 LowAir-GS 中的位置：

```text
图像数据
   ↓
COLMAP 特征提取与匹配
   ↓
相机内参、外参、稀疏点云
   ↓
3DGS 训练
```

检查命令：

```bash
colmap -h
```

第一阶段建议：

- 使用 Nerfstudio 示例数据时，可以先不深入配置 COLMAP；
- 使用自采图像时，需要通过 `ns-process-data images` 或 COLMAP 完成位姿估计；
- 如果学生机器没有 COLMAP，可由实验室服务器统一提供。

---

### 3.6 SuperSplat

用途：

- 打开导出的 Gaussian Splat 文件；
- 浏览不同视角；
- 检查漂浮点、破碎区域、空洞和模糊问题；
- 生成展示截图和演示素材。

第一阶段任务：

```text
1. 使用 SuperSplat 打开导出的 splat / ply 文件；
2. 截取至少 3 张不同视角图片；
3. 在实践报告中分析可视化效果和问题。
```

---

## 4. 选用工具说明

### 4.1 graphdeco-inria/gaussian-splatting

这是 3D Gaussian Splatting 的原始参考实现，适合后续论文复现和算法对比。

第一阶段不强制使用，原因是：

- 环境配置对新手不如 Nerfstudio 友好；
- Viewer 和数据处理流程需要更多手工配置；
- 对入门阶段而言，先理解 3DGS 流程比直接复现原始代码更重要。

建议使用阶段：

```text
第二阶段或第三阶段，用于和 Nerfstudio Splatfacto 做结果对比。
```

---

### 4.2 gsplat

gsplat 是面向 Gaussian Splatting 的 CUDA 加速库，Nerfstudio Splatfacto 使用它作为后端之一。

第一阶段学生不需要直接开发 gsplat，只需要知道：

```text
gsplat 负责高效完成 3D Gaussian 的投影、栅格化和训练相关计算。
```

后续如果做算法改进、加速或低显存优化，可以深入研究 gsplat。

---

### 4.3 OpenDroneMap / WebODM

用途：

- 处理无人机航拍图像；
- 生成正射影像、点云、三维网格、数字高程模型；
- 作为传统摄影测量基线，与 3DGS 的视觉效果对比。

第一阶段建议：

```text
先了解 WebODM 示例数据和处理结果，不要求所有学生安装完整 ODM。
```

后续适合做：

- 无人机摄影测量与 3DGS 的对比；
- 正射影像、点云、3DGS 模型的联合展示；
- 校园低空场景的测绘型产品生成。

---

### 4.4 FFmpeg

用途：

- 从无人机视频中抽帧；
- 压缩演示视频；
- 转换视频格式。

示例：

```bash
ffmpeg -i input.mp4 -vf fps=2 frames/frame_%05d.jpg
```

含义：每秒抽取 2 帧。

第一阶段如果只使用图片数据，可以暂不使用 FFmpeg。

---

### 4.5 OpenCV

用途：

- 图像清晰度检测；
- 图像缩放；
- 图像统计；
- 简单掩码处理。

典型模糊检测思路：

```text
读取图像
   ↓
转灰度图
   ↓
计算 Laplacian 方差
   ↓
方差过低则判定为模糊图像
```

后续可以在 `scripts/` 中实现自动筛图脚本。

---

## 5. 推荐数据集合

### 5.1 第一阶段必须优先使用的小数据

| 数据 | 来源 | 推荐用途 | 说明 |
|---|---|---|---|
| Nerfstudio poster | Nerfstudio 示例数据 | 环境验证 | 适合第一次跑通 `ns-train splatfacto` |
| Nerfstudio aspen | Nerfstudio 示例数据 | 真实场景训练 | 比 poster 更接近实际场景 |
| WebODM 示例数据 | WebODM Datasets | 无人机摄影测量入门 | 可理解无人机图像、点云、正射图关系 |

### 5.2 后续低空相关数据

| 数据集 | 用途 | 推荐阶段 |
|---|---|---|
| Mip-NeRF 360 | 新视角合成评价 | 第二阶段 |
| Tanks and Temples | 真实三维重建基准 | 第二阶段 |
| Mill-19 / Mega-NeRF | 大规模无人机/工业场景 | 第三阶段 |
| UrbanScene3D | 城市级三维场景 | 第三阶段 |
| UAVid | 无人机视频语义分割 | 语义增强阶段 |
| EuRoC MAV | 视觉惯性定位与 SLAM | 位姿估计扩展阶段 |
| TartanAir | 仿真深度、语义、位姿真值 | 鲁棒性验证阶段 |

### 5.3 自采数据建议

第一阶段可以增加一个很小的自采数据试验：

```text
场景：楼前小广场 / 一段道路 / 建筑入口
数量：50 到 150 张图像
设备：手机、相机或无人机
视角：平视 + 斜视 + 少量环绕
目标：验证是否可以完成位姿估计和基础训练
```

注意：

- 不要一开始采集大范围校园；
- 不要上传未经处理的人脸、车牌、敏感位置图片；
- 原始数据不要直接提交到 GitHub；
- 报告中只给数据目录、样例截图和采集说明。

---

## 6. 第一阶段算法模块对应关系

| 算法模块 | 对应工具 | 输入 | 输出 | 学生需要理解的重点 |
|---|---|---|---|---|
| 图像筛选 | OpenCV / 手工筛选 | 原始图像 | 清晰图像集 | 清晰度、曝光、重叠率 |
| 视频抽帧 | FFmpeg | 视频 | 图像帧 | 抽帧间隔不能过密也不能过稀 |
| SfM 位姿估计 | COLMAP | 多视角图像 | 相机参数、稀疏点云 | 特征匹配、位姿、稀疏点 |
| 3DGS 初始化 | Splatfacto / gsplat | COLMAP 点云和位姿 | 初始高斯 | 为什么初始几何很重要 |
| 3DGS 训练 | Splatfacto | 图像、位姿、初始高斯 | 优化后的高斯模型 | 误差反传、密度控制、颜色优化 |
| 模型导出 | Nerfstudio | 训练配置 | `.ply` / `.splat` | 模型文件如何用于展示 |
| 模型查看 | SuperSplat / Viewer | 高斯模型 | 可视化结果 | 如何判断模型好坏 |
| 报告整理 | Markdown | 命令、日志、截图 | 实践报告 | 证据链和可复现性 |

---

## 7. 第一阶段安装与检查命令

### 7.1 基础检查

```bash
git --version
python --version
pip --version
conda --version
nvidia-smi
```

### 7.2 Nerfstudio 检查

```bash
ns-install-cli
ns-train --help
ns-download-data --help
ns-viewer --help
ns-export --help
```

### 7.3 COLMAP 检查

```bash
colmap -h
```

### 7.4 FFmpeg 检查

```bash
ffmpeg -version
```

### 7.5 Python 图像处理库检查

```bash
python - <<'PY'
import cv2
import numpy as np
from PIL import Image
print('OpenCV:', cv2.__version__)
print('NumPy:', np.__version__)
print('PIL OK')
PY
```

---

## 8. 推荐安装顺序

推荐顺序如下：

```text
1. Git
2. Miniconda
3. NVIDIA 驱动与 CUDA 环境检查
4. PyTorch
5. Nerfstudio
6. COLMAP
7. FFmpeg
8. OpenCV / Pillow / NumPy 等 Python 工具
9. SuperSplat 在线或本地工具
```

说明：

- PyTorch 与 CUDA 的版本匹配应根据服务器实际环境决定；
- 不建议学生自行反复重装驱动；
- 服务器环境由教师或管理员统一配置更稳妥；
- 学生主要负责 Conda 环境、项目代码和实验记录。

---

## 9. 第一阶段工具选型结论

第一阶段推荐每个小组统一采用：

```text
训练框架：Nerfstudio
3DGS 方法：Splatfacto
位姿工具：COLMAP
展示工具：Nerfstudio Viewer + SuperSplat
报告格式：Markdown
协作方式：GitHub Branch + Pull Request
```

不建议第一阶段一上来就做：

```text
复杂大场景分块训练
多机分布式训练
完整语义高斯场景
复杂 SLAM 系统
移动端部署
高难度论文复现
```

第一阶段的目标是：

```text
让每个学生看见完整链路，理解关键算法，提交可复现实践报告。
```

---

## 10. 工具与资料链接

| 名称 | 链接 |
|---|---|
| LowAir-GS | https://github.com/SWJTU-AI-Lab/LowAir-GS |
| Nerfstudio | https://docs.nerf.studio/ |
| Nerfstudio Splatfacto | https://docs.nerf.studio/nerfology/methods/splat.html |
| COLMAP | https://colmap.github.io/ |
| OpenDroneMap 文档 | https://docs.opendronemap.org/ |
| WebODM 示例数据 | https://webodm.org/datasets/ |
| SuperSplat | https://github.com/playcanvas/supersplat |
| SuperSplat 在线编辑器 | https://superspl.at/editor |
| GaussianSplats3D | https://github.com/mkkellogg/GaussianSplats3D |
| gsplat | https://docs.gsplat.studio/ |
| 3DGS 官方参考实现 | https://github.com/graphdeco-inria/gaussian-splatting |
| FFmpeg | https://ffmpeg.org/ |
| OpenCV | https://opencv.org/ |
| CloudCompare | https://www.cloudcompare.org/ |
| MeshLab | https://www.meshlab.net/ |
