# 第一阶段入门级实践方案

本文档面向第一次接触 3D Gaussian Splatting、COLMAP 和低空三维重建的学生，目标是在第一阶段完成一个“可复现、可展示、可提交报告”的入门级实践闭环。

## 1. 阶段定位

第一阶段不是要求学生直接完成复杂低空场景创新算法，而是先完成三件事：

```text
跑通工具链 → 理解核心算法 → 形成可检查的实践报告
```

建议周期：**2 周到 3 周**。

建议对象：SRTP 项目新成员、本科生科研训练成员、研究生课程实践成员。

最终成果：

| 成果 | 要求 |
|---|---|
| 环境记录 | 能说明本机或服务器环境、GPU、CUDA、Python、Nerfstudio 等版本 |
| 数据记录 | 至少使用一个公开样例数据集，整理数据来源和目录结构 |
| 训练结果 | 至少完成一次 Splatfacto / 3DGS 训练 |
| 可视化结果 | 能在 Viewer 或 SuperSplat 中浏览结果 |
| 实验对比 | 至少完成一组小型对比实验，如训练步数、图像数量、分辨率等 |
| 实践报告 | 按照报告模板提交 Markdown 报告和截图证据 |

---

## 2. 第一阶段总体流程

```text
准备 GitHub 协作环境
        ↓
配置 Conda / Python / CUDA / Nerfstudio
        ↓
下载公开样例数据
        ↓
运行 Splatfacto / 3DGS 基线训练
        ↓
使用 Viewer 或 SuperSplat 查看模型
        ↓
完成一组小型对比实验
        ↓
整理命令、日志、截图和问题
        ↓
提交实践报告 Pull Request
```

---

## 3. 推荐开源工具

第一阶段建议先使用工具成熟、文档相对完整、学生容易上手的组合。

### 3.1 必选工具

| 工具 | 作用 | 学生需要掌握的程度 |
|---|---|---|
| Git / GitHub | 代码下载、分支管理、Pull Request 提交 | 会 clone、branch、commit、push、PR |
| Miniconda / Conda | Python 环境管理 | 会创建和激活环境 |
| Nerfstudio | 训练、可视化和评估 NeRF / 3DGS 模型 | 会使用 `ns-download-data`、`ns-train`、`ns-viewer`、`ns-export` |
| Splatfacto | Nerfstudio 中的 3D Gaussian Splatting 实现 | 会跑通一个公开数据集训练 |
| COLMAP | SfM 位姿估计和稀疏点云生成 | 会理解其作用，第一阶段可先不深入调参 |
| SuperSplat | 3D Gaussian Splat 模型浏览、检查、编辑和发布 | 会打开导出的 `.ply` / `.splat` 文件 |

### 3.2 选用工具

| 工具 | 适合用途 |
|---|---|
| graphdeco-inria/gaussian-splatting | 原始 3DGS 参考实现，适合后续论文复现 |
| OpenDroneMap / WebODM | 无人机摄影测量基线，适合处理航拍数据 |
| FFmpeg | 视频抽帧，适合后续处理无人机视频 |
| Python + OpenCV | 图像筛选、模糊检测、分辨率调整、数据统计 |
| MeshLab / CloudCompare | 点云和几何结果检查，作为传统三维重建辅助工具 |

第一阶段推荐组合：

```text
Nerfstudio Splatfacto + COLMAP + SuperSplat
```

---

## 4. 第一阶段涉及的核心算法

学生不需要一开始推导完整数学细节，但应理解每个模块“为什么存在、输入是什么、输出是什么”。

### 4.1 图像预处理

输入：无人机图像、手机图像、公开视频帧或公开样例数据。

主要处理：

- 图像筛选：剔除严重模糊、曝光过度、曝光不足的图像；
- 去冗余：视频抽帧时避免连续帧过密；
- 分辨率调整：降低训练显存占用；
- 数据命名：统一图像文件名和目录结构；
- 隐私处理：避免上传人员、人脸、车牌、敏感位置等未脱敏内容。

第一阶段可以只做最简单的筛选和整理，重点是保证工具链能跑通。

### 4.2 SfM 位姿估计

SfM，即 Structure-from-Motion，主要用于从多张有重叠视角的图像中估计相机姿态和稀疏三维点云。

简化流程如下：

```text
输入多张图像
   ↓
特征点提取
   ↓
图像特征匹配
   ↓
相机位姿估计
   ↓
三角化生成稀疏点云
   ↓
Bundle Adjustment 全局优化
   ↓
输出相机内参、外参和稀疏点云
```

对 LowAir-GS 来说，COLMAP 的输出非常关键，因为 3DGS 通常需要相机位姿和初始点云来获得更稳定的训练效果。

### 4.3 3D Gaussian Splatting

3DGS 用大量三维高斯表示场景，每个高斯可以粗略理解为一个可学习的“小椭球”。这些高斯共同表达场景的几何、颜色和透明度。

每个 3D Gaussian 通常包含：

```text
位置：在三维空间中的中心点
形状：尺度和旋转，决定高斯椭球的方向和大小
颜色：可用球谐函数表示不同视角下的颜色变化
透明度：决定该高斯对最终图像的贡献
```

训练时，系统会根据相机位姿把三维高斯投影到二维图像上，与真实图像进行比较，然后不断优化高斯参数。

简化训练流程：

```text
COLMAP 稀疏点云初始化高斯
        ↓
根据相机位姿渲染当前视角图像
        ↓
与真实图像计算误差
        ↓
反向传播优化高斯位置、形状、颜色、透明度
        ↓
增加、删除或调整高斯
        ↓
得到可实时浏览的 3DGS 场景
```

### 4.4 结果评价

第一阶段建议采用“定性评价 + 简单定量指标”。

| 指标 | 说明 | 第一阶段要求 |
|---|---|---|
| 可浏览性 | 模型能否在 Viewer / SuperSplat 中正常打开 | 必须完成 |
| 视觉完整性 | 建筑、道路、树木是否明显破碎 | 必须观察 |
| 训练耗时 | 从开始训练到得到可用结果的时间 | 必须记录 |
| 显存占用 | 训练时 GPU 显存使用情况 | 尽量记录 |
| PSNR / SSIM / LPIPS | 新视角合成常用指标 | 有评估条件时记录 |
| 模型大小 | 导出的 `.ply` / `.splat` 文件大小 | 建议记录 |

---

## 5. 推荐数据集

第一阶段数据集选择原则：

```text
小规模优先、公开数据优先、可复现优先、低门槛优先
```

### 5.1 入门样例数据

| 数据集 | 推荐程度 | 用途 |
|---|---|---|
| Nerfstudio `poster` 示例 | 强烈推荐 | 快速验证 Nerfstudio 是否安装成功 |
| Nerfstudio `aspen` 示例 | 推荐 | 测试更复杂的真实场景训练 |
| WebODM `banana` 数据 | 推荐 | 小型 3D 物体/场景摄影测量入门 |
| WebODM `mygla` 数据 | 推荐 | WebODM 页面标注为 Good starter set，适合无人机数据入门 |
| WebODM `aukerman` / `bellus` 数据 | 可选 | 航拍图像量更大，适合小组进一步练习 |

### 5.2 进阶公开数据

| 数据集 | 用途 | 第一阶段建议 |
|---|---|---|
| Mip-NeRF 360 | 新视角合成评价基准 | 了解即可，不建议一开始做完整实验 |
| Tanks and Temples | 真实三维重建基准 | 了解即可 |
| Mill-19 / Mega-NeRF | 大规模无人机场景 | 后续大场景方向再使用 |
| UrbanScene3D | 城市场景和低空航拍 | 后续低空大场景方向再使用 |
| UAVid | 城市低空语义分割 | 后续语义增强方向再使用 |

### 5.3 第一阶段推荐组合

建议每个小组至少完成以下两类数据实验：

```text
实验 A：Nerfstudio poster 示例数据
实验 B：WebODM 小型航拍或 3D 示例数据
```

如果实验室已有校园低空图像，可以增加：

```text
实验 C：校园小场景 50 到 150 张图像试跑
```

校园自采数据第一阶段不追求规模，重点是验证数据目录、位姿估计和训练流程是否通顺。

---

## 6. 环境准备

### 6.1 硬件建议

| 配置 | 建议 |
|---|---|
| GPU | NVIDIA GPU，建议 8GB 显存以上；显存不足时降低分辨率或缩小数据规模 |
| CPU | 普通多核 CPU 即可 |
| 内存 | 建议 16GB 以上 |
| 硬盘 | 建议预留 30GB 以上空间 |
| 系统 | Ubuntu 20.04 / 22.04 更推荐；Windows 可使用 WSL2 或实验室服务器 |

如果学生个人电脑没有合适 GPU，可以采用：

```text
个人电脑：写代码、整理数据、写报告、查看小模型
实验室服务器：训练 3DGS 模型
```

### 6.2 GitHub 准备

```bash
git clone https://github.com/SWJTU-AI-Lab/LowAir-GS.git
cd LowAir-GS
git checkout -b student/<your-name>-stage1
```

### 6.3 Conda 环境准备

以下命令仅作为入门参考，具体 PyTorch / CUDA 版本应根据服务器环境调整。

```bash
conda create -n lowair-gs python=3.10 -y
conda activate lowair-gs

python --version
nvidia-smi
```

安装 Nerfstudio：

```bash
pip install --upgrade pip
pip install nerfstudio
ns-install-cli
```

检查命令是否可用：

```bash
ns-train --help
ns-download-data --help
ns-viewer --help
ns-export --help
```

检查 COLMAP：

```bash
colmap -h
```

如果 `colmap` 命令不可用，第一阶段可以先使用 Nerfstudio 已处理样例数据完成训练；COLMAP 由教师或管理员后续统一配置。

---

## 7. 实践任务设计

### 任务 1：跑通 Nerfstudio 示例数据

目标：验证环境是否可用，并理解训练、查看、导出流程。

```bash
ns-download-data nerfstudio --capture-name=poster
ns-train splatfacto --data data/nerfstudio/poster
```

训练过程中记录：

```text
开始时间：
结束时间：
GPU 型号：
显存占用：
训练步数：
Viewer 地址：
输出目录：
```

训练完成或训练中可通过 Viewer 查看效果。若在远程服务器训练，需要进行端口转发，默认 Viewer WebSocket 端口通常为 7007。

### 任务 2：导出 3DGS 模型

找到训练输出中的 `config.yml`，执行：

```bash
ns-export gaussian-splat \
  --load-config outputs/<experiment-name>/splatfacto/<timestamp>/config.yml \
  --output-dir exports/stage1_poster_splat
```

检查输出目录中是否出现 `.ply` 等文件。

记录：

```text
导出文件路径：
导出文件大小：
是否能打开：
打开工具：Nerfstudio Viewer / SuperSplat / 其他
```

### 任务 3：使用 SuperSplat 查看结果

可以使用在线 SuperSplat Editor 或本地部署 SuperSplat。

学生需要完成：

```text
1. 打开导出的 Gaussian Splat 文件
2. 截取至少 3 张不同视角截图
3. 记录是否存在漂浮点、空洞、破碎、模糊等问题
4. 说明这些问题可能来自数据、位姿、训练参数还是场景本身
```

### 任务 4：完成一组小型对比实验

第一阶段建议选择一个简单变量做对比，不要同时改变太多因素。

#### 方案 A：训练步数对比

| 实验编号 | 设置 | 记录内容 |
|---|---|---|
| E1-A | 默认训练较短时间 | 训练耗时、效果截图、模型大小 |
| E1-B | 增加训练时间或步数 | 训练耗时、效果截图、模型大小 |

#### 方案 B：图像数量对比

从同一数据集中选取不同数量图像：

| 实验编号 | 图像数量 | 预期观察 |
|---|---|---|
| E2-A | 50 张 | 是否能完成位姿和训练 |
| E2-B | 100 张 | 场景是否更完整 |
| E2-C | 全部图像 | 质量是否提升，耗时是否增加 |

#### 方案 C：分辨率对比

| 实验编号 | 分辨率设置 | 观察内容 |
|---|---|---|
| E3-A | 原始分辨率 | 质量、耗时、显存 |
| E3-B | 1/2 分辨率 | 质量变化、速度变化 |
| E3-C | 1/4 分辨率 | 是否出现细节丢失 |

第一阶段只要求完成其中一个方案。

### 任务 5：撰写实践报告并提交 PR

学生需要基于 `docs/STAGE1_PRACTICE_REPORT_TEMPLATE.md` 撰写报告。

建议提交位置：

```text
reports/stage1/<your-name>-stage1-report.md
assets/stage1/<your-name>/screenshots/
```

如果仓库暂未创建 `reports/` 和 `assets/stage1/` 目录，可以在 PR 中一并创建。截图数量不宜过多，建议只提交压缩后的关键截图；大视频和大模型文件不要提交到 GitHub。

---

## 8. 验收标准

### 8.1 基础验收

| Gate | 验收项 | 通过标准 |
|---|---|---|
| G1 | GitHub 协作 | 已创建个人分支并提交 PR |
| G2 | 环境可用 | 能运行 `ns-train --help` 和至少一个训练命令 |
| G3 | 数据准备 | 能说明所用数据集来源和目录结构 |
| G4 | 模型训练 | 至少完成一次 Splatfacto / 3DGS 训练 |
| G5 | 可视化 | 能提供 Viewer 或 SuperSplat 截图 |
| G6 | 对比实验 | 至少完成一组简单对比实验 |
| G7 | 实践报告 | 报告包含命令、日志、截图、问题分析和总结 |

### 8.2 加分项

| 加分项 | 说明 |
|---|---|
| 自采小场景数据 | 使用手机或无人机采集 50 到 150 张图像并试跑 |
| COLMAP 日志分析 | 能解释图像注册失败或点云稀疏的原因 |
| 模糊检测脚本 | 使用 OpenCV 实现简单的图像模糊筛选 |
| 网页展示 | 能把导出的模型放入网页或 SuperSplat 中展示 |
| 问题复盘 | 能清楚说明失败实验的原因，而不是只展示成功结果 |

---

## 9. 常见问题

### 9.1 训练很慢怎么办？

优先减少数据量和分辨率，不要一开始使用大场景。第一阶段以跑通流程为主，不以最高质量为目标。

### 9.2 COLMAP 失败怎么办？

常见原因包括：

- 图像重叠不足；
- 建筑墙面重复纹理过多；
- 图像模糊；
- 大量天空区域；
- 反光玻璃过多；
- 动态行人和车辆过多。

第一阶段可以先使用公开样例数据完成流程，再回头分析自采数据问题。

### 9.3 模型中有很多漂浮点怎么办？

可能原因包括：

- 位姿估计不准确；
- 背景天空区域太多；
- 图像中存在动态目标；
- 训练步数不足或参数不合适；
- 图像质量不稳定。

报告中应记录现象和可能原因，不要求第一阶段完全解决。

### 9.4 是否需要提交大文件？

不需要。GitHub 中只提交代码、配置、文档、少量截图和小型结果说明。原始数据、大模型文件、大视频应放在实验室服务器或网盘，并在报告中给出路径说明。

---

## 10. 第一阶段推荐时间安排

| 周次 | 任务 | 产出 |
|---|---|---|
| 第 1 周第 1 次 | GitHub 协作、环境说明、工具介绍 | 个人分支、环境记录 |
| 第 1 周第 2 次 | 跑通 Nerfstudio 示例数据 | 训练日志、Viewer 截图 |
| 第 2 周第 1 次 | 导出模型并用 SuperSplat 查看 | 导出文件记录、展示截图 |
| 第 2 周第 2 次 | 完成一个小型对比实验 | 对比表格、问题分析 |
| 第 3 周可选 | 自采小场景试跑 | 自采数据说明、失败/成功复盘 |

---

## 11. 参考资料

- Nerfstudio Splatfacto 文档：<https://docs.nerf.studio/nerfology/methods/splat.html>
- Nerfstudio 入门训练文档：<https://docs.nerf.studio/quickstart/first_nerf.html>
- COLMAP 官方文档：<https://colmap.github.io/>
- 3DGS 原始参考实现：<https://github.com/graphdeco-inria/gaussian-splatting>
- WebODM 示例数据集：<https://webodm.org/datasets/>
- SuperSplat Editor：<https://github.com/playcanvas/supersplat>
