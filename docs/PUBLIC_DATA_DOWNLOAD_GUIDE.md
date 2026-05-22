# 公开数据下载替代方案

> 适用场景：当 Nerfstudio 官网示例数据或 `ns-download-data nerfstudio --capture-name=poster` 等链接失效、下载很慢或网络不可达时，学生可以改用本文档中的公开数据源。

## 1. 基本判断

Nerfstudio 的 `ns-download-data` 很方便，但它依赖外部数据源。如果外部链接变化、访问超时或网络受限，第一阶段实践不要卡在这里。我们的原则是：

```text
优先保证工具链跑通，而不是执着于某一个示例数据。
```

因此，第一阶段推荐采用三类替代数据：

```text
A. WebODM 小型公开数据：适合无人机 / 摄影测量入门
B. Mip-NeRF 360 小场景数据：适合 3DGS / 新视角合成入门
C. 自采小规模图像：适合 LowAir-GS 低空场景试跑
```

---

## 2. 第一阶段推荐数据优先级

| 优先级 | 数据类型 | 推荐数据 | 适合用途 | 说明 |
|---|---|---|---|---|
| P0 | WebODM 小数据 | banana、mygla | 快速验证图像数据目录、COLMAP、ODM 思路 | 小、容易下载，适合第一周 |
| P1 | Mip-NeRF 360 小场景 | bonsai、counter、room、kitchen | 验证 3DGS 训练和渲染质量 | 数据较大，但标准化程度更好 |
| P2 | 3DGS 官方相关数据 | Tanks&Temples + Deep Blending COLMAP 数据 | 论文复现和后续对比 | 更适合第二阶段 |
| P3 | 自采校园小场景 | 50 到 150 张图像 | 验证低空三维重建流程 | 第一阶段可作为加分项 |

---

## 3. WebODM 公开数据

WebODM 提供了一批无人机航拍和摄影测量示例数据。第一阶段建议优先使用小数据，不要一上来选择几 GB 的大数据。

### 3.1 推荐数据

| 数据名 | 图像数 | 体量 | 推荐理由 |
|---|---:|---:|---|
| banana | 16 | 约 14 MB | 很小，适合快速测试数据目录和图像处理流程 |
| mygla | 29 | 约 150 MB | WebODM 页面标注为 Good starter set，适合入门 |
| aukerman | 77 | 约 543 MB | 数据量适中，可用于进一步练习 |
| bellus | 122 | 约 717 MB | 图像更多，适合小组进阶练习 |

### 3.2 手动下载方式

进入 WebODM 数据页面：

```text
https://webodm.org/datasets/
```

选择数据集后，通常会跳转到 GitHub、Google Drive、Dropbox 等下载位置。学生优先选择 GitHub 数据源，因为其目录结构更清晰，适合写报告。

### 3.3 GitHub ZIP 下载方式

可以直接下载 GitHub 仓库压缩包：

```bash
mkdir -p data/public/webodm
cd data/public/webodm

# banana 数据
wget -O banana.zip https://github.com/pierotofy/dataset_banana/archive/refs/heads/master.zip
unzip banana.zip -d banana

# mygla 数据
wget -O mygla.zip https://github.com/merkato/odm_mygla_dataset/archive/refs/heads/master.zip
unzip mygla.zip -d mygla

# aukerman 数据
wget -O aukerman.zip https://github.com/OpenDroneMap/odm_data_aukerman/archive/refs/heads/master.zip
unzip aukerman.zip -d aukerman
```

如果 `wget` 不可用，可以换成：

```bash
curl -L -o banana.zip https://github.com/pierotofy/dataset_banana/archive/refs/heads/master.zip
```

---

## 4. Mip-NeRF 360 替代数据

Mip-NeRF 360 是新视角合成和 3DGS 相关实验中常用的数据集。第一阶段不要下载全部数据，建议只选一个小场景。

### 4.1 推荐顺序

| 场景 | 体量 | 第一阶段建议 |
|---|---:|---|
| bonsai | 约 1.3 GB | 推荐，室内小场景，适合入门 |
| counter | 约 1.3 GB | 推荐，室内小场景，适合入门 |
| room | 约 1.6 GB | 推荐，室内场景，适合训练测试 |
| kitchen | 约 1.9 GB | 可选，体量略大 |
| garden / bicycle / stump | 4 GB 以上 | 不建议第一阶段直接使用 |

### 4.2 下载方式

```bash
mkdir -p data/public/mipnerf360
cd data/public/mipnerf360

# 推荐先下载 bonsai 或 counter
wget https://data.ciirc.cvut.cz/public/projects/2023NerfBaselines/data/gaussian-splatting/mipnerf360/bonsai.zip
unzip bonsai.zip -d bonsai

wget https://data.ciirc.cvut.cz/public/projects/2023NerfBaselines/data/gaussian-splatting/mipnerf360/counter.zip
unzip counter.zip -d counter
```

如果网络不稳定，建议使用断点续传：

```bash
wget -c https://data.ciirc.cvut.cz/public/projects/2023NerfBaselines/data/gaussian-splatting/mipnerf360/bonsai.zip
```

---

## 5. 3DGS 官方相关数据

3D Gaussian Splatting 官方参考实现提供了 Tanks&Temples 和 Deep Blending 的 COLMAP 数据包，适合后续论文复现和标准对比。

第一阶段不建议所有学生都下载，但可以由教师或组长下载一次，放到实验室服务器公共目录。

```bash
mkdir -p data/public/graphdeco
cd data/public/graphdeco
wget -c https://repo-sam.inria.fr/fungraph/3d-gaussian-splatting/datasets/input/tandt_db.zip
unzip tandt_db.zip -d tandt_db
```

建议服务器共享路径：

```text
/data/lowair-gs/public_datasets/graphdeco/tandt_db/
```

---

## 6. 使用自采图像替代官方示例

如果公开数据下载不顺利，也可以直接使用手机或无人机采集一个极小场景。

### 6.1 场景建议

```text
建筑入口、走廊一角、实验室桌面、小广场一角、楼前台阶
```

### 6.2 采集要求

| 项目 | 建议 |
|---|---|
| 图像数量 | 50 到 150 张 |
| 拍摄方式 | 围绕目标缓慢移动，保持 60% 以上重叠 |
| 图像质量 | 避免严重模糊、过曝、欠曝 |
| 视角覆盖 | 正面、侧面、斜视、少量俯视 |
| 动态目标 | 尽量避开行人、车辆、摇晃树叶 |
| 隐私处理 | 不上传人脸、车牌、敏感地点信息 |

### 6.3 使用 Nerfstudio 处理自采图像

假设图像放在：

```text
data/local/campus_test/images/
```

可执行：

```bash
ns-process-data images \
  --data data/local/campus_test/images \
  --output-dir data/processed/campus_test

ns-train splatfacto --data data/processed/campus_test
```

如果 COLMAP 注册失败，报告中要记录失败原因，不要求第一阶段一定成功。

---

## 7. 仓库脚本下载方式

本仓库提供了一个简单下载脚本：

```bash
python scripts/download_public_dataset.py --list
```

下载 banana：

```bash
python scripts/download_public_dataset.py --dataset webodm_banana --output data/public --extract
```

下载 mygla：

```bash
python scripts/download_public_dataset.py --dataset webodm_mygla --output data/public --extract
```

下载 Mip-NeRF 360 bonsai：

```bash
python scripts/download_public_dataset.py --dataset mipnerf360_bonsai --output data/public --extract
```

说明：

- 下载脚本只做“便利入口”，不是唯一方式；
- 如果脚本失败，可以直接复制文档中的 URL 手动下载；
- 大文件不应提交到 GitHub。

---

## 8. 第一阶段训练流程替换建议

原始写法如果失败：

```bash
ns-download-data nerfstudio --capture-name=poster
ns-train splatfacto --data data/nerfstudio/poster
```

建议替换为：

```bash
# 方式一：使用自采或 WebODM 图像，先做 COLMAP / Nerfstudio 数据处理
ns-process-data images \
  --data data/public/webodm/banana/dataset_banana-master/images \
  --output-dir data/processed/webodm_banana

ns-train splatfacto --data data/processed/webodm_banana
```

或：

```bash
# 方式二：使用已经包含 COLMAP / transforms 文件的数据，直接训练
ns-train splatfacto --data <processed_dataset_path>
```

注意：不同数据包的目录结构不完全一致，学生需要先确认图像目录实际位置。常见图像目录名称包括：

```text
images/
input/
train/images/
```

---

## 9. 数据登记模板

每次使用公开数据或自采数据，都要在报告中登记：

```text
数据名称：
数据来源：
下载地址：
下载日期：
图像数量：
数据大小：
是否包含相机位姿：是 / 否
是否需要 COLMAP：是 / 否
本地路径：
使用目的：环境验证 / 位姿估计 / 3DGS 训练 / 对比实验
问题记录：
```

---

## 10. 教师建议

建议教师或组长在实验室服务器上统一准备以下公共数据：

```text
/data/lowair-gs/public_datasets/
├── webodm_banana/
├── webodm_mygla/
├── mipnerf360_bonsai/
└── graphdeco_tandt_db/
```

学生只读访问公共数据，在自己的工作目录中生成中间结果和训练输出：

```text
/data/lowair-gs/group_01/
├── processed/
├── outputs/
├── exports/
└── reports/
```

这样可以避免每个学生重复下载大文件，也可以减少 GitHub 仓库污染。
