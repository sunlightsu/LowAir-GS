/**
 * RenderWidget.cpp — Demo-02 OpenGL 渲染窗口实现
 */

#include "RenderWidget.h"
#include <QOpenGLContext>

RenderWidget::RenderWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat fmt;
    fmt.setVersion(4, 1);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setDepthBufferSize(24);
    fmt.setSamples(4);  // 4x MSAA
    setFormat(fmt);
    setMinimumSize(640, 480);
}

RenderWidget::~RenderWidget()
{
    makeCurrent();
    m_meshRenderer.cleanup();
    m_gridRenderer.cleanup();
    m_axisRenderer.cleanup();
    m_bboxRenderer.cleanup();
    doneCurrent();
}

// ─── 初始化 ──────────────────────────────────────────────────────────────────

void RenderWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.12f, 0.13f, 0.16f, 1.0f);  // 深蓝灰背景

    m_meshRenderer.initialize();
    m_gridRenderer.initialize(100, 5);
    m_axisRenderer.initialize(30.0f);
    m_bboxRenderer.initialize();

    m_camera.resetToDefault();
}

void RenderWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

// ─── 渲染 ────────────────────────────────────────────────────────────────────

void RenderWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = (height() > 0) ? (float)width() / height() : 1.0f;
    QMatrix4x4 view = m_camera.viewMatrix();
    QMatrix4x4 proj = m_camera.projectionMatrix(aspect);

    // 地面网格
    m_gridRenderer.render(view, proj);

    // 坐标轴
    m_axisRenderer.render(view, proj);

    // 模型
    if (m_hasModel) {
        QMatrix4x4 modelMat;
        modelMat.translate(m_centerOffset[0], m_centerOffset[1], m_centerOffset[2]);
        m_meshRenderer.render(modelMat, view, proj, m_wireframe);

        // 包围盒
        if (m_showBBox) {
            m_bboxRenderer.render(modelMat, view, proj);
        }
    }
}

// ─── 模型加载 ────────────────────────────────────────────────────────────────

void RenderWidget::loadModel(const ModelAsset &asset)
{
    makeCurrent();
    m_meshRenderer.uploadModel(asset);

    for (int i = 0; i < 3; ++i) {
        m_bboxMin[i]      = asset.bboxMin[i];
        m_bboxMax[i]      = asset.bboxMax[i];
        m_centerOffset[i] = asset.centerOffset[i];
    }

    // 设置包围盒（已应用中心偏移）
    m_bboxRenderer.setBox(
        asset.bboxMin[0] + m_centerOffset[0],
        asset.bboxMin[1] + m_centerOffset[1],
        asset.bboxMin[2] + m_centerOffset[2],
        asset.bboxMax[0] + m_centerOffset[0],
        asset.bboxMax[1] + m_centerOffset[1],
        asset.bboxMax[2] + m_centerOffset[2]
    );

    m_hasModel = true;
    fitToModel();
    doneCurrent();

    emit modelLoaded(asset.totalVertices, asset.totalTriangles);
    update();
}

// ─── 视图控制 ────────────────────────────────────────────────────────────────

void RenderWidget::setWireframe(bool on)
{
    m_wireframe = on;
    update();
}

void RenderWidget::setShowBBox(bool on)
{
    m_showBBox = on;
    update();
}

void RenderWidget::resetView()
{
    m_camera.resetToDefault();
    update();
}

void RenderWidget::fitToModel()
{
    if (!m_hasModel) return;
    // 包围盒已应用中心偏移，模型居中后包围盒中心在原点附近
    float cx = (m_bboxMin[0] + m_bboxMax[0]) * 0.5f + m_centerOffset[0];
    float cy = (m_bboxMin[1] + m_bboxMax[1]) * 0.5f + m_centerOffset[1];
    float cz = (m_bboxMin[2] + m_bboxMax[2]) * 0.5f + m_centerOffset[2];
    float dx = m_bboxMax[0] - m_bboxMin[0];
    float dy = m_bboxMax[1] - m_bboxMin[1];
    float dz = m_bboxMax[2] - m_bboxMin[2];
    float diag = std::sqrt(dx*dx + dy*dy + dz*dz);
    m_camera.setTarget(QVector3D(cx, cy, cz));
    m_camera.setRadius(diag * 1.5f > 1.0f ? diag * 1.5f : 1.0f);
    m_camera.setAzimuth(45.0f);
    m_camera.setElevation(30.0f);
    update();
}

// ─── 鼠标交互 ────────────────────────────────────────────────────────────────

void RenderWidget::mousePressEvent(QMouseEvent *e)
{
    m_lastMousePos  = e->pos();
    m_pressedButton = e->button();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPoint delta = e->pos() - m_lastMousePos;
    m_lastMousePos = e->pos();

    if (m_pressedButton == Qt::LeftButton) {
        // 左键：旋转
        m_camera.orbit(-delta.x() * 0.4f, delta.y() * 0.4f);
    } else if (m_pressedButton == Qt::RightButton) {
        // 右键：平移
        m_camera.pan(delta.x(), delta.y());
    }
    update();
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_pressedButton = Qt::NoButton;
}

void RenderWidget::wheelEvent(QWheelEvent *e)
{
    float delta = e->angleDelta().y() / 120.0f;
    m_camera.zoom(delta);
    update();
}
