# Demo-07: Game Event Evaluation - 二次开发指南

## 1. 架构概览
Demo-07 构建在原生 OpenGL 渲染引擎之上，主要增加了业务逻辑层和特效渲染层。
- **业务逻辑层**：`MissionConfig` (任务解析)、`TrajectoryReplay` (轨迹回放)、`GameEventEngine` (事件碰撞检测与评分)。
- **特效渲染层**：`Effect` 基类及其派生类（`GlowEffectRenderer`, `ParticleEffectRenderer`, `VirtualExplosionRenderer` 等），负责在 3D 空间中绘制动态视觉效果。
- **UI/HUD 层**：`HudRenderer` 负责在 OpenGL 视口上方绘制 2D 的文字弹窗和信息覆盖。

## 2. 任务与数据结构
任务数据由两个文件组成，位于 `sample_data/demo07_game/`：
1. **mission_config.json**：定义了任务名称、目标点（`targets`，包含坐标、半径、触发特效类型）和评分规则（`score_rules`）。
2. **replay_track.jsonl**：JSON Lines 格式的轨迹文件，每行代表一帧（包含 `t` 和 `pos` 数组）。

### 2.1 添加新的目标点
直接修改 `mission_config.json` 的 `targets` 数组。`effect` 字段支持 `glow`, `pulse`, `particle`, `explosion`。

## 3. 扩展视觉特效
如果现有的特效不能满足需求，您可以按照以下步骤添加自定义特效：

### 3.1 创建特效渲染器
1. 继承 `Effect` 类（或直接实现渲染接口），创建新的渲染器（如 `LaserBeamRenderer`）。
2. 在该类中实现 OpenGL 的初始化（VBO/VAO）、Shader 编译和 `render(const QMatrix4x4 &view, const QMatrix4x4 &proj)` 方法。

### 3.2 注册特效
1. 在 `Effect.h` 的 `EffectType` 枚举中添加新类型（如 `LaserBeam`）。
2. 在 `MissionConfig.cpp` 的解析逻辑中，将 JSON 字符串映射到新的枚举值。
3. 在 `RenderWidget::spawnEffect` 中，添加对应的 `switch-case` 分支，实例化并保存您的渲染器。

### 3.3 粒子系统扩展
现有的 `ParticleEffectRenderer` 使用了简单的基于 CPU 的运动学更新（在 `update()` 方法中计算重力和速度）。对于更复杂的粒子系统（如成千上万个粒子），建议将位置更新逻辑迁移到 Compute Shader 或 Transform Feedback 中执行。

## 4. HUD 渲染扩展
`HudRenderer` 使用 `QPainter` 结合 `QOpenGLPaintDevice` 在 3D 场景之上绘制 2D 内容。
如果需要添加新的 UI 元素（如雷达图、小地图）：
1. 在 `HudRenderer::render` 中获取 `QPainter`。
2. 使用标准的 Qt 2D 绘图 API（`drawText`, `drawRect`, `drawImage`）进行绘制。
3. 注意：`QPainter` 的坐标系原点在左上角。
