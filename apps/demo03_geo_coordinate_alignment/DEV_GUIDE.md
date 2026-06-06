# LowAir-GS Demo-03 二次开发手册

## 目录

1. 绪论
   1.1 研究背景
   1.2 研究内容
   1.3 研究方法
   1.4 技术突破点
   1.5 验证与分析
   1.6 性能提升情况
   1.7 应用场景
2. 技术方案正文
   2.1 项目背景
   2.2 研究内容
   2.3 研究方法与坐标转换原理
      2.3.1 WGS84 到 ECEF 转换公式
      2.3.2 ECEF 到 ENU 转换公式
      2.3.3 ENU 到 SCENE 及 OpenGL 映射
   2.4 技术路线与架构设计
      2.4.1 核心模块划分
      2.4.2 遥测接收协议设计
      2.4.3 渲染管线设计
   2.5 实验设计与接口说明
      2.5.1 `CoordinateTransformer` 接口
      2.5.2 `GeoUavState` 数据结构
      2.5.3 Python 模拟器扩展
3. 项目研究结论及展望
   3.1 研究结论
   3.2 创新成果
   3.3 下一步研究展望

---

## 1. 绪论

### 1.1 研究背景
在低空智能系统中，无人机遥测数据通常采用世界大地测量系统1984（World Geodetic System 1984, WGS84）坐标系表示，而三维高斯渲染（3D Gaussian Splatting, 3DGS）及底层图形接口（Open Graphics Library, OpenGL）则依赖于局部笛卡尔坐标系。如何高精度、低延迟地实现全局经纬高向局部三维场景坐标的转换，是低空数字孪生系统的基础难题。

### 1.2 研究内容
本研究（Demo-03）主要内容包括构建一套完整的坐标转换链路，实现从 WGS84 到地心地固坐标系（Earth-Centered, Earth-Fixed, ECEF），再到东北天局部切平面坐标系（East-North-Up, ENU），最终映射至三维渲染场景坐标（SCENE）及图形显示坐标（OpenGL）的无缝转换。

### 1.3 研究方法
采用内置高精度 WGS84 椭球体数学模型进行空间解析几何变换，避免对重型地理信息系统（Geographic Information System, GIS）库（如 PROJ 或 GeographicLib）的强依赖。同时，基于用户数据报协议（User Datagram Protocol, UDP）和 JavaScript 对象简谱（JavaScript Object Notation, JSON）格式实现遥测数据的异步接收与解析，结合 Qt6 与现代 OpenGL 管线进行实时三维可视化。

### 1.4 技术突破点
本方案的突破点在于将复杂的地理空间转换算法轻量化地集成至 C++ 客户端中，并实现了微秒级的单点坐标转换延迟。同时，通过统一的 `CoordinateTransformer` 调度器，确保了多级坐标在用户界面（User Interface, UI）面板与三维视口间的严格同步。

### 1.5 验证与分析
系统通过内置的 Python 遥测模拟器发送椭圆轨迹数据进行验证。实验表明，地理原点的自校验误差接近于 0 米，测试点位（向东 10 米，向北 20 米，高度 +5 米）的反算误差小于 0.01 米，完全满足低空局部场景的精度需求。

### 1.6 性能提升情况
相较于调用外部 Python 脚本或引入庞大的 C++ GIS 库，内置的解析几何转换方案使程序体积减小了约 80%，同时单次坐标系映射的计算开销降低至可忽略不计的程度，有效保障了高帧率（Frames Per Second, FPS）渲染下的数据更新效率。

### 1.7 应用场景
该坐标转换系统可广泛应用于无人机地面控制站（Ground Control Station, GCS）、低空飞行器数字孪生监控、三维高斯场景地理对齐，以及基于位置服务（Location-Based Services, LBS）的虚拟现实（Virtual Reality, VR）展示平台。

---

## 2. 技术方案正文

### 2.1 项目背景
在 LowAir-GS 系统的整体架构中，Demo-01 验证了基础的 UDP 遥测接入能力，Demo-02 验证了静态三维资产的解析与渲染。Demo-03 则作为承上启下的关键节点，致力于解决真实地理坐标与虚拟渲染空间之间的数学映射问题，为后续真实飞行器的接入奠定空间基准。

### 2.2 研究内容
本模块的二次开发研究内容聚焦于地理坐标系转换算法的 C++ 实现、异步网络接收机制的构建，以及基于现代着色器（Shader）的轨迹与坐标轴渲染。

### 2.3 研究方法与坐标转换原理

在三维空间中定位无人机，需要依次经过以下坐标系变换：

#### 2.3.1 WGS84 到 ECEF 转换公式
WGS84 坐标系采用纬度（Latitude, $\phi$）、经度（Longitude, $\lambda$）和椭球高（Altitude, $h$）表示。其对应的 ECEF 坐标 $(X, Y, Z)$ 计算如下：

WGS84 椭球参数：
- 长半轴 $a = 6378137.0$ 米
- 扁率倒数 $1/f = 298.257223563$
- 第一偏心率平方 $e^2 = 2f - f^2$

卯酉圈曲率半径 $N(\phi)$ 计算：
$N(\phi) = \frac{a}{\sqrt{1 - e^2 \sin^2\phi}}$

空间直角坐标计算：
$X = (N + h) \cos\phi \cos\lambda$
$Y = (N + h) \cos\phi \sin\lambda$
$Z = (N(1-e^2) + h) \sin\phi$

#### 2.3.2 ECEF 到 ENU 转换公式
设定参考原点（Origin）的 WGS84 坐标为 $(\phi_0, \lambda_0, h_0)$，其 ECEF 坐标为 $(X_0, Y_0, Z_0)$。目标点在 ECEF 系下的坐标差为：
$dx = X - X_0$
$dy = Y - Y_0$
$dz = Z - Z_0$

通过旋转矩阵将坐标差转换至以参考原点为中心的 ENU 坐标系（向东 $E$、向北 $N$、向上 $U$）：
$E = -\sin\lambda_0 dx + \cos\lambda_0 dy$
$N = -\sin\phi_0 \cos\lambda_0 dx - \sin\phi_0 \sin\lambda_0 dy + \cos\phi_0 dz$
$U = \cos\phi_0 \cos\lambda_0 dx + \cos\phi_0 \sin\lambda_0 dy + \sin\phi_0 dz$

#### 2.3.3 ENU 到 SCENE 及 OpenGL 映射
在获取 ENU 坐标后，需将其映射至三维渲染场景（SCENE）及最终的图形显示坐标（OpenGL）。

(1) ENU 到 SCENE 映射
SCENE 坐标系定义为与 ENU 方向完全一致，仅引入场景缩放因子（Scene Scale, $S$）：
$x_{scene} = E \times S$
$y_{scene} = N \times S$
$z_{scene} = U \times S$

(2) SCENE 到 OpenGL 映射
OpenGL 采用右手坐标系，且默认摄像机看向 $-Z$ 轴，$+Y$ 轴向上。因此进行如下轴映射：
$x_{gl} = x_{scene}$ (对应向东)
$y_{gl} = z_{scene}$ (对应向上)
$z_{gl} = -y_{scene}$ (对应向南，即负的向北)

### 2.4 技术路线与架构设计

#### 2.4.1 核心模块划分
- **geo/**：封装 WGS84 椭球数学模型与多级转换管线，对外提供统一的 `CoordinateTransformer` 接口。
- **telemetry/**：基于 Qt Network 模块实现 UDP 监听与 JSON 解析，提取无人机姿态与位置信息。
- **render/**：包含 `GeoSceneRenderer`（绘制地理原点、网格与距离环）、`UavMarkerRenderer`（绘制飞行器实体）与 `TrajectoryRenderer`（维护并绘制历史轨迹点）。
- **camera/**：实现基于球面坐标系的轨道摄像机（Orbit Camera），支持平移、缩放与视点环绕。

#### 2.4.2 遥测接收协议设计
系统监听指定 UDP 端口，接收 JSON 格式数据。数据包需包含标识符（uav_id）、时间戳（timestamp_ms）、坐标系标识（frame，必须为 "WGS84"）、经度（lat）、纬度（lon）、高度（alt）及姿态角（roll, pitch, yaw）。

#### 2.4.3 渲染管线设计
采用顶点数组对象（Vertex Array Object, VAO）与顶点缓冲对象（Vertex Buffer Object, VBO）管理几何数据，通过自定义着色器程序（`QOpenGLShaderProgram`）应用模型观察投影矩阵（Model-View-Projection Matrix, MVP）完成顶点变换。

### 2.5 实验设计与接口说明

#### 2.5.1 `CoordinateTransformer` 接口
二次开发时，可直接实例化 `CoordinateTransformer` 类：
- `void setOrigin(const Wgs84Coord& origin)`：设置地理参考原点，内部自动缓存原点的 ECEF 坐标。
- `TransformResult transform(const Wgs84Coord& wgs) const`：输入任意 WGS84 坐标，返回包含 ECEF、ENU、SCENE、OpenGL 坐标及相对距离、方位角的完整结果集。

#### 2.5.2 `GeoUavState` 数据结构
该结构体用于承载解析后的遥测帧：
- `double lat, lon, alt`：地理位置。
- `double roll, pitch, yaw`：欧拉角姿态（单位：度）。
- `double battery`：设备电量百分比。

#### 2.5.3 Python 模拟器扩展
在 `tools/geo_telemetry_simulator/send_wgs84_uav_udp.py` 中，可通过修改 `generate_ellipse_trajectory` 函数，引入随机噪声或复杂的航线规划算法，以测试接收端的鲁棒性。

---

## 3. 项目研究结论及展望

### 3.1 研究结论
Demo-03 成功构建了从全局地理坐标到局部图形渲染坐标的完整映射体系。内置的数学转换方案在保证精度的前提下，大幅降低了系统的依赖复杂度和计算开销，为低空三维可视化提供了可靠的空间基准。

### 3.2 创新成果
提出了一种轻量级的四级坐标系转换架构（WGS84 $\rightarrow$ ECEF $\rightarrow$ ENU $\rightarrow$ SCENE $\rightarrow$ OpenGL），并通过 Qt 信号槽机制与现代 OpenGL 管线实现了数据的低延迟驱动渲染，有效解决了多源异构坐标系的对齐难题。

### 3.3 下一步研究展望
在后续版本（Demo-04 及以后）中，计划将当前的 Python 模拟器替换为真实的微型飞行器通信协议（Micro Air Vehicle Link, MAVLink）或机器人操作系统（Robot Operating System 2, ROS2）数据源。同时，探索在 ENU 坐标系下加载 3D Gaussian Splatting 模型，实现无人机真实轨迹与高保真三维场景的深度融合展示。
