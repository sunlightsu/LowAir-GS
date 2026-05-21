# 学生使用说明与权限管理建议

本文档用于说明学生如何参与 LowAir-GS 项目，以及教师或项目管理员应如何配置 GitHub、服务器和数据权限。

## 1. 核心原则

**不要给学生管理员密码。**

学生不需要 GitHub 仓库管理员权限，也不需要服务器 root 密码或系统管理员密码。项目管理应采用“最小权限原则”：学生只获得完成任务所必需的权限，所有代码修改通过分支、Pull Request 和教师审核进入主分支。

推荐权限模型如下：

```text
教师 / 项目负责人：仓库 Admin 或 Maintain 权限
学生组长：Write 权限，可创建分支和提交 Pull Request
普通学生：Fork 仓库或获得 Write 权限，但不能直接改 main
外部协作者：Fork + Pull Request
服务器使用者：普通 Linux 用户账号，不授予 root 密码
```

---

## 2. GitHub 参与方式

### 2.1 推荐方式一：Fork + Pull Request

适合刚加入项目、还不熟悉 GitHub 协作流程的学生。

基本流程：

```text
Fork 仓库
   ↓
Clone 到本地
   ↓
新建自己的开发分支
   ↓
完成代码、文档或实验结果修改
   ↓
提交 commit
   ↓
推送到自己的 fork
   ↓
向主仓库提交 Pull Request
   ↓
教师或组长审核后合并
```

示例命令：

```bash
git clone https://github.com/<your-name>/LowAir-GS.git
cd LowAir-GS
git checkout -b student/<name>-baseline-demo

# 修改代码或文档后
git add .
git commit -m "docs: add baseline experiment notes"
git push origin student/<name>-baseline-demo
```

然后在 GitHub 页面上发起 Pull Request。

### 2.2 推荐方式二：组织内协作分支

适合已经确定加入课题组、能够稳定参与项目开发的学生。

教师可以把学生加入仓库协作者，并给予 `Write` 权限，但要求所有学生不得直接向 `main` 分支提交内容，必须使用个人分支或任务分支。

推荐分支命名：

```text
student/<姓名或学号>-<任务名>
exp/<实验编号>-<实验主题>
docs/<文档主题>
fix/<问题编号>-<简短说明>
```

示例：

```text
student/zhangsan-colmap-demo
exp/e01-image-number-ablation
docs/dataset-guide
fix/colmap-path-error
```

---

## 3. 不建议的做法

以下做法不推荐：

- 不要把 GitHub 仓库管理员权限直接给所有学生；
- 不要多人共用一个 GitHub 账号；
- 不要把 GitHub Token、服务器密码、无人机账号密码写入 README、代码或提交记录；
- 不要让学生直接向 `main` 分支提交未经审核的代码；
- 不要把大型原始数据、训练输出模型、视频文件直接提交到 GitHub；
- 不要在公共仓库中上传含有敏感位置、人员隐私或未脱敏的校园数据。

---

## 4. 服务器和实验环境权限

3D Gaussian Splatting 通常需要 GPU 环境。若使用实验室服务器，建议采用普通用户账号或容器环境，而不是共享管理员密码。

### 4.1 推荐服务器权限设置

```text
教师 / 管理员：拥有 sudo 或管理员权限
学生：普通 Linux 用户账号
每组一个工作目录：/data/lowair-gs/group_xxx/
每组一个 Conda 环境或容器环境
重要数据目录只读，实验输出目录可写
```

示例目录：

```text
/data/lowair-gs/
├── public_datasets/          # 公共数据，只读
├── group_01/                 # 第一组工作目录
├── group_02/                 # 第二组工作目录
├── group_03/                 # 第三组工作目录
└── shared_models/            # 公共模型或示例输出，只读或受控写入
```

### 4.2 学生是否需要 sudo

一般不需要。

学生需要的是：

- 能登录服务器；
- 能使用 GPU；
- 能进入自己的工作目录；
- 能创建 Conda 环境或使用已配置好的 Conda / Docker 环境；
- 能读取公共数据集；
- 能写入自己的实验输出目录。

如果确实需要安装系统依赖，由教师或管理员统一安装。学生不应长期持有 root 密码。

---

## 5. 推荐的学生启动流程

学生第一次使用本项目时，建议按以下流程开展。

### 第一步：克隆仓库

```bash
git clone https://github.com/SWJTU-AI-Lab/LowAir-GS.git
cd LowAir-GS
```

如果采用 Fork 模式，则克隆自己的 fork 仓库。

### 第二步：阅读项目文档

建议先阅读：

```text
README.md
docs/STUDENT_USAGE.md
docs/DATASET_GUIDE.md
docs/EXPERIMENT_PLAN.md
docs/STUDENT_TASKS.md
```

如果部分文档还未创建，则根据 README 中的阶段计划逐步补充。

### 第三步：创建个人分支

```bash
git checkout -b student/<your-name>-getting-started
```

### 第四步：完成一个小任务

建议从文档类或复现实验类小任务开始，例如：

- 整理一个公开数据集下载说明；
- 跑通一个 Nerfstudio Splatfacto 示例；
- 记录一次 COLMAP 处理流程；
- 上传一张流程图或实验截图；
- 编写一次实验日志。

### 第五步：提交 Pull Request

```bash
git add .
git commit -m "docs: add getting started notes"
git push origin student/<your-name>-getting-started
```

然后在 GitHub 页面上提交 Pull Request，由教师或组长审核。

---

## 6. 学生任务类型

学生可以从以下几类任务中选择：

| 任务类型 | 典型内容 | 适合对象 |
|---|---|---|
| 文档任务 | 数据集说明、安装记录、实验步骤、问题总结 | 新手同学 |
| 数据任务 | 图像采集、抽帧、筛选、目录整理、数据说明 | 数据采集组 |
| 位姿任务 | COLMAP / OpenDroneMap 处理、重建日志分析 | 视觉几何方向 |
| 训练任务 | Nerfstudio / 3DGS 训练、参数对比、显存记录 | 算法与工程方向 |
| 展示任务 | SuperSplat、网页展示、漫游视频、截图整理 | 前端与展示方向 |
| 语义任务 | SAM 分割、语义掩码、目标标注、对象查询 | 语义理解方向 |

---

## 7. 推荐验收方式

每个任务都应提交可检查的证据，而不是只口头说明。

推荐每个 Pull Request 包含：

```text
1. 做了什么
2. 修改了哪些文件
3. 如何运行或复现
4. 结果截图或日志
5. 仍然存在的问题
6. 下一步建议
```

实验类任务建议提交：

```text
实验命令
环境信息
数据目录
关键参数
训练日志
输出截图
结果表格
问题记录
```

---

## 8. 数据和模型文件管理

GitHub 不适合直接保存大型原始数据和训练输出结果。建议：

- 小型说明文件、脚本、配置文件可以提交到 GitHub；
- 原始图片、视频、大模型文件、训练输出的 PLY / SPLAT 文件不要直接提交；
- 大文件可保存到实验室服务器、网盘、对象存储或 Git LFS；
- 仓库中只保留数据索引、下载说明、目录结构和少量示例图。

推荐数据说明格式：

```text
数据名称：campus_building_01
采集时间：YYYY-MM-DD
采集设备：无人机 / 手机 / 相机型号
图像数量：xxx 张
场景范围：教学楼正面、小广场、道路
数据位置：服务器路径或下载链接
隐私处理：是否包含人员、车牌、敏感位置
负责人：xxx
```

---

## 9. 管理员给学生的最小权限清单

### GitHub 权限

优先选择：

```text
普通参与者：Fork + Pull Request
核心学生：Write 权限 + 分支保护
教师或负责人：Maintain / Admin 权限
```

### 服务器权限

优先选择：

```text
普通 Linux 用户账号
指定工作目录读写权限
GPU 使用权限
Conda 环境使用权限
公共数据只读权限
```

不建议提供：

```text
root 密码
管理员账号
共享 GitHub Token
共享服务器账号
长期 sudo 权限
```

---

## 10. 建议的教学组织方式

第一周可以让学生只做三件事：

```text
1. 注册 GitHub，并学会 Fork / Clone / Branch / Pull Request
2. 阅读 README 和本使用说明
3. 每人提交一个小型文档 PR，完成协作流程训练
```

第二周再进入环境搭建和公开数据集复现。这样做可以先把协作规范建立起来，后续项目推进会更顺畅。
