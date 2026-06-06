#include "RenderWidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLContext>
#include <QImage>

RenderWidget::RenderWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    QSurfaceFormat fmt;
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setSamples(4);
    setFormat(fmt);
}

RenderWidget::~RenderWidget() {
    makeCurrent();
    m_meshRenderer.cleanup();
    m_gaussianRenderer.cleanup();
    m_gridRenderer.cleanup();
    m_axisRenderer.cleanup();
    m_bboxRenderer.cleanup();
    doneCurrent();
}

void RenderWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);

    m_meshRenderer.initialize();
    m_gaussianRenderer.initialize();
    m_gridRenderer.initialize();
    m_axisRenderer.initialize();
    m_bboxRenderer.initialize();
    m_camera.reset();
}

void RenderWidget::resizeGL(int w, int h) {
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, float(w) / float(h ? h : 1), 0.01f, 5000.0f);
}

void RenderWidget::paintGL() {
    m_fps.tick();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view = m_camera.viewMatrix();
    QMatrix4x4 identity;
    identity.setToIdentity();

    // 1. 网格和坐标轴
    QMatrix4x4 vp = m_proj * view;
    if (m_layerCtrl && m_layerCtrl->showGrid())
        m_gridRenderer.render(vp);
    if (m_layerCtrl && m_layerCtrl->showAxis())
        m_axisRenderer.render(vp);

    // 2. Mesh 层
    if (m_meshUploaded && m_layerCtrl && m_layerCtrl->isMeshVisible()) {
        MeshRenderMode meshMode = MeshRenderMode::Solid;
        if (m_layerCtrl->isMeshWireframeOverlay() || m_layerCtrl->meshWireframe())
            meshMode = MeshRenderMode::Wireframe;
        float meshOp = m_layerCtrl->meshOpacity();
        if (meshOp < 1.0f) meshMode = MeshRenderMode::Transparent;
        m_meshRenderer.render(identity, view, m_proj, meshMode, meshOp);

        // Wireframe Comparison 模式：先画实体再画线框叠加
        if (m_layerCtrl->displayMode() == FusionDisplayMode::WireframeComparison) {
            m_meshRenderer.render(identity, view, m_proj, MeshRenderMode::Wireframe, 0.5f);
        }
    }

    // 3. Gaussian 层
    if (m_gaussianUploaded && m_layerCtrl && m_layerCtrl->isGaussianVisible()) {
        QMatrix4x4 gaussModel = m_fusionTf ? m_fusionTf->gaussianModelMatrix() : identity;
        GaussianRenderMode gMode = m_layerCtrl->gaussianSplatMode()
                                   ? GaussianRenderMode::Splat
                                   : GaussianRenderMode::Point;
        m_gaussianRenderer.render(gaussModel, view, m_proj, gMode,
                                  m_layerCtrl->gaussianOpacity());
    }

    // 4. 包围盒
    if (m_layerCtrl) {
        if (m_meshUploaded && m_layerCtrl->showMeshBbox()) {
            m_bboxRenderer.setBox(m_meshBboxMin, m_meshBboxMax);
            m_bboxRenderer.render(m_proj * view * identity);
        }
        if (m_gaussianUploaded && m_layerCtrl->showGaussianBbox()) {
            QMatrix4x4 gaussModel = m_fusionTf ? m_fusionTf->gaussianModelMatrix() : identity;
            m_bboxRenderer.setBox(m_gaussianBboxMin, m_gaussianBboxMax);
            m_bboxRenderer.render(m_proj * view * gaussModel);
        }
    }

    emit frameRendered(m_fps.fps(), m_fps.frameTimeMs());
    update();  // 持续刷新
}

void RenderWidget::uploadMesh(const MeshAsset& asset) {
    makeCurrent();
    m_meshRenderer.upload(asset);
    m_meshBboxMin = asset.bboxMin;
    m_meshBboxMax = asset.bboxMax;
    m_meshUploaded = true;
    doneCurrent();
    update();
}

void RenderWidget::uploadGaussian(const GaussianCloud& cloud) {
    makeCurrent();
    m_gaussianRenderer.upload(cloud);
    m_gaussianBboxMin = cloud.bboxMin();
    m_gaussianBboxMax = cloud.bboxMax();
    m_gaussianUploaded = true;
    doneCurrent();
    update();
}

void RenderWidget::clearMesh() {
    m_meshUploaded = false;
    update();
}

void RenderWidget::clearGaussian() {
    m_gaussianUploaded = false;
    update();
}

void RenderWidget::resetCamera() {
    m_camera.reset();
    update();
}

void RenderWidget::fitToScene(const QVector3D& center, float radius) {
    m_camera.setTarget(center);
    m_camera.setDistance(radius * 2.5f);
    update();
}

void RenderWidget::takeScreenshot(const QString& path) {
    QImage img = grabFramebuffer();
    img.save(path);
}

void RenderWidget::mousePressEvent(QMouseEvent* e) {
    m_lastMousePos = e->pos();
}

void RenderWidget::mouseMoveEvent(QMouseEvent* e) {
    QPoint delta = e->pos() - m_lastMousePos;
    m_lastMousePos = e->pos();
    if (e->buttons() & Qt::LeftButton) {
        m_camera.orbit(delta.x() * 0.4f, delta.y() * 0.4f);
    } else if (e->buttons() & Qt::RightButton) {
        m_camera.pan(-delta.x() * 0.005f, delta.y() * 0.005f);
    }
    update();
}

void RenderWidget::wheelEvent(QWheelEvent* e) {
    float delta = e->angleDelta().y() / 120.0f;
    m_camera.zoom(-delta * 0.15f);
    update();
}
