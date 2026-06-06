#pragma once
#include <string>

// 融合显示模式
enum class FusionDisplayMode {
    MeshOnly,           // 只显示 Mesh
    GaussianOnly,       // 只显示 Gaussian
    DualFusion,         // 双源叠加显示
    WireframeComparison // Mesh 线框 + Gaussian 半透明
};

// 图层控制器：管理各图层的可见性、透明度和渲染模式
class FusionLayerController {
public:
    FusionLayerController();

    // 显示模式
    FusionDisplayMode displayMode() const { return m_displayMode; }
    void setDisplayMode(FusionDisplayMode mode) { m_displayMode = mode; }

    // Mesh 图层
    bool  meshEnabled()    const { return m_meshEnabled; }
    float meshOpacity()    const { return m_meshOpacity; }
    bool  meshWireframe()  const { return m_meshWireframe; }
    bool  showMeshBbox()   const { return m_showMeshBbox; }
    void  setMeshEnabled(bool v)   { m_meshEnabled = v; }
    void  setMeshOpacity(float v)  { m_meshOpacity = v; }
    void  setMeshWireframe(bool v) { m_meshWireframe = v; }
    void  setShowMeshBbox(bool v)  { m_showMeshBbox = v; }

    // Gaussian 图层
    bool  gaussianEnabled()   const { return m_gaussianEnabled; }
    float gaussianOpacity()   const { return m_gaussianOpacity; }
    bool  gaussianSplatMode() const { return m_gaussianSplatMode; }
    bool  showGaussianBbox()  const { return m_showGaussianBbox; }
    void  setGaussianEnabled(bool v)    { m_gaussianEnabled = v; }
    void  setGaussianOpacity(float v)   { m_gaussianOpacity = v; }
    void  setGaussianSplatMode(bool v)  { m_gaussianSplatMode = v; }
    void  setShowGaussianBbox(bool v)   { m_showGaussianBbox = v; }

    // 场景辅助
    bool showGrid()  const { return m_showGrid; }
    bool showAxis()  const { return m_showAxis; }
    bool showFps()   const { return m_showFps; }
    void setShowGrid(bool v) { m_showGrid = v; }
    void setShowAxis(bool v) { m_showAxis = v; }
    void setShowFps(bool v)  { m_showFps = v; }

    // 根据当前显示模式计算实际的 Mesh / Gaussian 可见性
    bool isMeshVisible()     const;
    bool isGaussianVisible() const;
    bool isMeshWireframeOverlay() const;

    std::string displayModeName() const;

private:
    FusionDisplayMode m_displayMode = FusionDisplayMode::DualFusion;

    bool  m_meshEnabled    = true;
    float m_meshOpacity    = 1.0f;
    bool  m_meshWireframe  = false;
    bool  m_showMeshBbox   = true;

    bool  m_gaussianEnabled    = true;
    float m_gaussianOpacity    = 0.75f;
    bool  m_gaussianSplatMode  = true;
    bool  m_showGaussianBbox   = true;

    bool  m_showGrid = true;
    bool  m_showAxis = true;
    bool  m_showFps  = true;
};
