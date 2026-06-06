/**
 * RenderWidget.h — Demo-02 OpenGL 渲染窗口
 *
 * 继承 QOpenGLWidget，管理所有渲染器和相机。
 * 支持鼠标轨道控制、模型加载、线框切换、包围盒显示。
 */

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QMouseEvent>
#include <QWheelEvent>
#include <memory>

#include "camera/OrbitCamera.h"
#include "render/MeshRenderer.h"
#include "render/GridRenderer.h"
#include "render/AxisRenderer.h"
#include "render/BoundingBoxRenderer.h"
#include "asset/ModelAsset.h"

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core
{
    Q_OBJECT
public:
    explicit RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget() override;

    void loadModel(const ModelAsset &asset);
    void setWireframe(bool on);
    void setShowBBox(bool on);
    void resetView();
    void fitToModel();

signals:
    void modelLoaded(uint32_t vertices, uint32_t triangles);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

private:
    OrbitCamera          m_camera;
    MeshRenderer         m_meshRenderer;
    GridRenderer         m_gridRenderer;
    AxisRenderer         m_axisRenderer;
    BoundingBoxRenderer  m_bboxRenderer;

    bool m_wireframe = false;
    bool m_showBBox  = false;
    bool m_hasModel  = false;

    // 当前模型包围盒（用于 fitToModel）
    float m_bboxMin[3] = {-1,-1,-1};
    float m_bboxMax[3] = { 1, 1, 1};
    float m_centerOffset[3] = {0,0,0};

    // 鼠标交互
    QPoint m_lastMousePos;
    Qt::MouseButton m_pressedButton = Qt::NoButton;
};
