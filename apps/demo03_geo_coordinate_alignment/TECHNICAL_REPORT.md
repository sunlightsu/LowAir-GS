# Demo-03: Geo-Coordinate Alignment - 技术报告

## 1. 摘要
本技术报告阐述了 Demo-03 (Geo-Coordinate Alignment) 的核心技术实现。该 Demo 解决了无人机地面站开发中的关键问题：如何将无人机传回的 WGS84 全球地理坐标（经度、纬度、海拔）精确转换为 3D 渲染引擎使用的局部笛卡尔坐标系（ENU：东-北-天）。

## 2. 坐标系定义
在无人机监控系统中，涉及两种主要的坐标系：
- **WGS84 坐标系**：全球定位系统使用的标准参考椭球体坐标系，由纬度（$\phi$）、经度（$\lambda$）和椭球高（$h$）组成。
- **ENU 局部坐标系**：以地球表面某一点为原点的局部笛卡尔坐标系。X 轴指向正东（East），Y 轴指向正北（North），Z 轴指向天顶（Up）。这是 3D 渲染（如 Qt3D）最常用的物理坐标系。

## 3. 核心算法与数学模型
### 3.1 坐标转换流程
WGS84 到 ENU 的转换不能直接进行，通常需要经过 ECEF（Earth-Centered, Earth-Fixed，地心地固坐标系）作为中间桥梁：
$$ (Lat, Lon, Alt) \xrightarrow{\text{WGS84 to ECEF}} (X_e, Y_e, Z_e) \xrightarrow{\text{ECEF to ENU}} (X_{enu}, Y_{enu}, Z_{enu}) $$

### 3.2 GeographicLib 的应用
为了保证计算的严密性和高精度，本项目并未手动实现上述复杂的矩阵变换，而是引入了业界标准的 `GeographicLib` 库。
在 `GeoMath.cpp` 中，我们封装了 `GeographicLib::LocalCartesian` 类：
1. **初始化原点**：使用 `Reset(lat0, lon0, alt0)` 设定局部 ENU 坐标系的原点。
2. **正向转换 (Forward)**：使用 `Forward(lat, lon, alt, x, y, z)` 将目标点的 WGS84 坐标转换为相对于原点的 ENU 坐标 $(x, y, z)$。
3. **反向转换 (Reverse)**：使用 `Reverse(x, y, z, lat, lon, alt)` 将局部坐标还原为 WGS84 坐标（本 Demo 暂未在 UI 暴露，但底层已支持）。

## 4. 软件架构设计
- **GeoMath 模块**：封装底层地理计算，提供干净的 C++ 接口，隔离了对 `GeographicLib` 的直接依赖。
- **RenderWidget 模块**：负责 3D 可视化。利用 Qt3D 构建了包含坐标轴（QExtrudedTextMesh）、原点基准（Sphere）、目标点（Sphere）和连线（QGeometryRenderer）的场景。
- **MainWindow 模块**：处理 UI 输入、调用 `GeoMath` 进行计算，并将结果分发给 UI 显示和 `RenderWidget` 渲染。

## 5. 精度与性能验证
- **计算精度**：`GeographicLib` 采用高精度的椭球积分算法，其转换误差在亚毫米级别，完全满足低空无人机（通常飞行范围在几十公里内）的精度要求。
- **性能开销**：单次坐标转换耗时在微秒级，即使在 100Hz 的高频遥测数据更新下，坐标转换的计算开销也几乎可以忽略不计。

## 6. 结论
Demo-03 成功构建了 WGS84 与局部 3D 空间之间的精确数学映射关系。这一能力是后续 Demo-06（双场景融合）和 Demo-07（事件评估）中将无人机真实 GPS 轨迹映射到虚拟 3D 场景中的核心基础。
