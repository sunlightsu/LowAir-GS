#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QTextEdit>
#include <QSplitter>
#include <memory>

#include "RenderWidget.h"
#include "mesh/MeshAsset.h"
#include "mesh/AssimpMeshLoader.h"
#include "gaussian/GaussianCloud.h"
#include "gaussian/GaussianPlyLoader.h"
#include "fusion/FusionConfig.h"
#include "fusion/FusionTransform.h"
#include "fusion/FusionLayerController.h"
#include "fusion/FusionMetrics.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onOpenMesh();
    void onOpenGaussian();
    void onLoadFusionConfig();
    void onSaveFusionConfig();
    void onMeshOnly();
    void onGaussianOnly();
    void onDualFusion();
    void onWireframeComparison();
    void onResetAlignment();
    void onCenterAlign();
    void onFitToScene();
    void onBboxScaleMatch();
    void onApplyTransform();
    void onTakeScreenshot();
    void onFrameRendered(float fps, float frameMs);
    void onMeshOpacityChanged(int val);
    void onGaussianOpacityChanged(int val);

private:
    void buildUi();
    QWidget* buildLeftPanel();
    QWidget* buildRightPanel();
    void applyDarkTheme();
    void updateStatsPanel();
    void updateTransformSpinboxes();
    void log(const QString& msg);

    // 渲染窗口
    RenderWidget* m_renderWidget = nullptr;

    // 左侧面板控件
    QLabel*      m_lblMeshPath     = nullptr;
    QLabel*      m_lblGaussianPath = nullptr;
    QSlider*     m_sliderMeshOp    = nullptr;
    QSlider*     m_sliderGsOp      = nullptr;
    QCheckBox*   m_chkMeshEnabled  = nullptr;
    QCheckBox*   m_chkGsEnabled    = nullptr;
    QCheckBox*   m_chkMeshBbox     = nullptr;
    QCheckBox*   m_chkGsBbox       = nullptr;
    QCheckBox*   m_chkGrid         = nullptr;
    QCheckBox*   m_chkAxis         = nullptr;
    QCheckBox*   m_chkGsSplat      = nullptr;
    QComboBox*   m_cmbMeshMode     = nullptr;

    // 对齐控制
    QDoubleSpinBox* m_spnTx = nullptr;
    QDoubleSpinBox* m_spnTy = nullptr;
    QDoubleSpinBox* m_spnTz = nullptr;
    QDoubleSpinBox* m_spnRx = nullptr;
    QDoubleSpinBox* m_spnRy = nullptr;
    QDoubleSpinBox* m_spnRz = nullptr;
    QDoubleSpinBox* m_spnScale = nullptr;

    // 右侧统计面板
    QLabel* m_lblMeshStats    = nullptr;
    QLabel* m_lblGsStats      = nullptr;
    QLabel* m_lblAlignStats   = nullptr;
    QLabel* m_lblPerfStats    = nullptr;
    QLabel* m_lblFusionMode   = nullptr;

    // 日志
    QTextEdit* m_logEdit = nullptr;

    // 数据
    MeshAsset          m_meshAsset;
    GaussianCloud      m_gaussianCloud;
    FusionConfig       m_fusionConfig;
    FusionTransform    m_fusionTransform;
    FusionLayerController m_layerCtrl;
    FusionMetrics      m_metrics;

    AssimpMeshLoader   m_meshLoader;
};
