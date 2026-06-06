#pragma once
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QMatrix4x4>
#include <QPoint>
#include <memory>

#include "mesh/MeshAsset.h"
#include "mesh/MeshRenderer.h"
#include "gaussian/GaussianCloud.h"
#include "gaussian/GaussianBillboardRenderer.h"
#include "fusion/FusionLayerController.h"
#include "fusion/FusionTransform.h"
#include "render/GridRenderer.h"
#include "render/AxisRenderer.h"
#include "render/BoundingBoxRenderer.h"
#include "camera/OrbitCamera.h"
#include "perf/FpsCounter.h"

class RenderWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
    Q_OBJECT
public:
    explicit RenderWidget(QWidget* parent = nullptr);
    ~RenderWidget() override;

    void uploadMesh(const MeshAsset& asset);
    void uploadGaussian(const GaussianCloud& cloud);
    void clearMesh();
    void clearGaussian();

    void setLayerController(FusionLayerController* ctrl) { m_layerCtrl = ctrl; }
    void setFusionTransform(FusionTransform* tf)         { m_fusionTf  = tf;   }

    void resetCamera();
    void fitToScene(const QVector3D& meshCenter, float sceneRadius);
    void takeScreenshot(const QString& path);

    float fps()         const { return m_fps.fps(); }
    float frameTimeMs() const { return m_fps.frameTimeMs(); }

signals:
    void frameRendered(float fps, float frameMs);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void wheelEvent(QWheelEvent* e) override;

private:
    MeshRenderer              m_meshRenderer;
    GaussianBillboardRenderer m_gaussianRenderer;
    GridRenderer              m_gridRenderer;
    AxisRenderer              m_axisRenderer;
    BoundingBoxRenderer       m_bboxRenderer;
    OrbitCamera               m_camera;
    FpsCounter                m_fps;

    FusionLayerController* m_layerCtrl = nullptr;
    FusionTransform*       m_fusionTf  = nullptr;

    bool m_meshUploaded     = false;
    bool m_gaussianUploaded = false;

    // 包围盒信息（用于绘制）
    QVector3D m_meshBboxMin, m_meshBboxMax;
    QVector3D m_gaussianBboxMin, m_gaussianBboxMax;

    QPoint m_lastMousePos;
    QMatrix4x4 m_proj;
};
