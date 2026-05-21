# LowAir-GS

**LowAir-GS: Low-Altitude 3D Gaussian Splatting Reconstruction System**  
**低空高斯三维重建系统：基于 3D Gaussian Splatting 的低空复杂场景三维重建与可视化系统**

LowAir-GS is an open-source research and teaching project for reconstructing low-altitude 3D scenes from UAV or ground-view image sequences. The project focuses on a practical pipeline from image acquisition, camera pose estimation, 3D Gaussian Splatting reconstruction, scene visualization, and semantic extension.

LowAir-GS 面向低空智能、无人机巡检、校园三维建模与教学科研实践，目标是帮助学生从真实低空影像出发，完成“数据采集—位姿估计—三维高斯重建—可视化展示—语义增强”的完整工程链路。

---

## 1. Project Motivation / 项目背景

Low-altitude intelligent systems require accurate, lightweight, and visually realistic 3D scene representations. Traditional photogrammetry can produce point clouds, meshes, orthophotos, and DEM products, while 3D Gaussian Splatting provides high-quality novel-view rendering and interactive visualization. LowAir-GS aims to combine these strengths and build a reproducible student-friendly workflow for low-altitude 3D reconstruction.

低空智能系统需要具备对建筑、道路、树木、广场、停车区和应急通道等场景要素的三维感知与表达能力。传统摄影测量适合生成点云、网格、正射影像等几何产品，而 3D Gaussian Splatting 更适合形成高真实感、可实时浏览的新视角渲染结果。本项目希望将二者结合，形成适合 SRTP、课程实践和科研探索的低空三维重建开源项目。

---

## 2. Research Goals / 研究目标

- Build a complete low-altitude 3D reconstruction pipeline based on UAV images and 3D Gaussian Splatting.
- Compare different image acquisition strategies, including oblique-view, orbit-view, grid-view, and multi-height flight paths.
- Evaluate reconstruction quality under different image quantities, resolutions, motion blur levels, and scene scales.
- Develop a lightweight visualization workflow for interactive demonstration and teaching.
- Explore semantic-enhanced Gaussian scenes for low-altitude intelligent applications.

中文目标：

- 构建基于无人机低空影像和 3D Gaussian Splatting 的三维重建流程；
- 分析不同低空采集策略对重建质量的影响；
- 对图像数量、分辨率、模糊程度、场景尺度等因素进行实验评估；
- 建设可用于课程展示和项目答辩的三维可视化系统；
- 探索面向道路、建筑、树木、车辆、可降落区域等对象的语义高斯场景表达。

---

## 3. System Pipeline / 系统流程

```text
UAV / Camera Images
        ↓
Image Selection, Frame Extraction, Blur Filtering, Mask Preprocessing
        ↓
Camera Pose Estimation with COLMAP / OpenDroneMap
        ↓
Sparse Point Cloud and Camera Parameters
        ↓
3D Gaussian Splatting Training
        ↓
PLY / SPLAT Model Export
        ↓
Web Viewer, Video Rendering, Interactive Scene Exploration
        ↓
Semantic Labeling, Object Query, Low-Altitude Scene Understanding
```

对应中文流程：

```text
无人机影像 / 手持相机影像
        ↓
图像筛选、视频抽帧、模糊剔除、语义掩码预处理
        ↓
COLMAP / OpenDroneMap 位姿估计
        ↓
稀疏点云与相机内外参
        ↓
3D Gaussian Splatting 模型训练
        ↓
PLY / SPLAT 模型导出
        ↓
网页可视化、漫游视频、交互式场景浏览
        ↓
语义标注、目标查询、低空场景理解
```

---

## 4. Recommended Open-Source Tools / 推荐开源工具

| Category | Tools | Purpose |
|---|---|---|
| Camera Pose Estimation | COLMAP, OpenDroneMap, WebODM | SfM, MVS, sparse point cloud, UAV photogrammetry baseline |
| 3DGS Training | graphdeco-inria/gaussian-splatting, Nerfstudio Splatfacto, gsplat | Gaussian scene optimization and novel-view rendering |
| Visualization | Nerfstudio Viewer, SuperSplat, GaussianSplats3D | Interactive 3DGS viewing and web presentation |
| Semantic Extension | SAM, LangSplat, Gaussian Grouping, SAGA | Semantic masks, open-vocabulary scene understanding, Gaussian segmentation |
| Large Scene Extension | CityGaussian, VastGaussian | Scene partitioning, large-scale low-altitude reconstruction |

---

## 5. Repository Structure / 仓库结构

```text
LowAir-GS/
├── README.md
├── docs/
│   ├── ROADMAP.md
│   ├── DATASET_GUIDE.md
│   ├── EXPERIMENT_PLAN.md
│   └── STUDENT_TASKS.md
├── scripts/
│   └── .gitkeep
├── data/
│   └── .gitkeep
├── assets/
│   └── .gitkeep
├── outputs/
│   └── .gitkeep
└── .gitignore
```

Directory meaning:

- `docs/`: project plans, dataset guide, experiment design, and student task cards.
- `scripts/`: future preprocessing, training, evaluation, and visualization scripts.
- `data/`: local dataset workspace. Large datasets should not be committed to GitHub.
- `assets/`: figures, diagrams, screenshots, and demo images for README or reports.
- `outputs/`: local model outputs, rendered videos, evaluation tables, and logs. Large outputs should not be committed.

---

## 6. Minimal Viable Project / 最小可行版本

The first version of LowAir-GS should focus on one small but complete scene:

> one building + one small square + one road segment

Minimum deliverables:

| Deliverable | Requirement |
|---|---|
| Dataset | 200 to 600 low-altitude images with oblique, orbit, and overview views |
| Pose Estimation | COLMAP reconstruction with most images successfully registered |
| 3DGS Model | A browsable Gaussian scene in a viewer |
| Experiments | At least three comparison groups, such as image number, resolution, and blur filtering |
| Visualization | One web demo or viewer recording, plus one 1-minute fly-through video |
| Report | Data acquisition, reconstruction pipeline, experiment results, problems, and future work |

---

## 7. Suggested Student Workflow / 学生实践流程

1. Run a public sample dataset to understand the 3DGS workflow.
2. Collect a small campus low-altitude image dataset.
3. Use COLMAP or OpenDroneMap to estimate camera poses.
4. Train a baseline 3D Gaussian Splatting model.
5. Export the model and view it with a 3DGS viewer.
6. Compare different acquisition and preprocessing strategies.
7. Add semantic labels or simple object-level annotations.
8. Prepare experiment reports, figures, videos, and presentation slides.

---

## 8. Research Directions / 可扩展研究方向

- Low-altitude image acquisition strategy for 3DGS reconstruction.
- Blur filtering and dynamic-object masking for UAV image sequences.
- Traditional photogrammetry and 3DGS hybrid reconstruction.
- Large-scale scene partitioning and multi-block Gaussian fusion.
- Semantic Gaussian scene construction for low-altitude intelligent systems.
- Lightweight web visualization and model compression for teaching demos.

---

## 9. Current Status / 当前状态

This repository is currently in the initialization stage. The first development milestone is to complete the baseline documentation, dataset guide, and a reproducible 3DGS reconstruction demo.

当前仓库处于初始化阶段。第一阶段重点是完善项目文档、数据集说明、实验设计，并跑通一个可复现的低空三维高斯重建 Demo。

---

## 10. Citation / 引用说明

If this project is helpful for your course project, SRTP work, or research prototype, please cite or acknowledge this repository:

```bibtex
@misc{lowairgs2026,
  title        = {LowAir-GS: Low-Altitude 3D Gaussian Splatting Reconstruction System},
  author       = {SWJTU-AI-Lab},
  year         = {2026},
  howpublished = {\url{https://github.com/SWJTU-AI-Lab/LowAir-GS}}
}
```

---

## 11. License / 开源协议

The license is to be determined by the project maintainers.

开源协议待项目维护者确认后补充。若用于课程教学与 SRTP 实践，建议后续明确选择 MIT、Apache-2.0 或其他合适协议。
