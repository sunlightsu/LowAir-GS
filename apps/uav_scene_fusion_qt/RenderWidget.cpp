#include "RenderWidget.h"
#include "scene/SimpleScene.h"
#include "render/UavRenderer.h"
#include "render/TrajectoryRenderer.h"
#include "render/ObjModelRenderer.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFile>

RenderWidget::RenderWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_scene(nullptr)
    , m_uavRenderer(nullptr)
    , m_trajRenderer(nullptr)
    , m_modelBunny(nullptr)
    , m_modelTerrain(nullptr)
    , m_cameraDistance(120.0f)
    , m_cameraPitch(45.0f)
    , m_cameraYaw(20.0f)
{
}

RenderWidget::~RenderWidget()
{
    makeCurrent();
    delete m_scene;
    delete m_uavRenderer;
    delete m_trajRenderer;
    delete m_modelBunny;
    delete m_modelTerrain;
    doneCurrent();
}

void RenderWidget::initializeGL()
{
    initializeOpenGLFunctions();
    
    // 设置深色背景
    glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_scene = new SimpleScene(this);
    m_uavRenderer = new UavRenderer(this);
    m_trajRenderer = new TrajectoryRenderer(this);

    m_scene->initialize();
    m_uavRenderer->initialize();
    m_trajRenderer->initialize();

    // 加载摄影测量三维模型
    // 尝试从多个路径查找模型文件
    QStringList searchPaths = {
        QCoreApplication::applicationDirPath() + "/../../sample_data/scene",
        QDir::homePath() + "/LowAir-GS/sample_data/scene",
        "/home/ubuntu/LowAir-GS/sample_data/scene"
    };

    auto findModel = [&](const QString& name) -> QString {
        for (const auto& p : searchPaths) {
            QString full = p + "/" + name;
            if (QFile::exists(full)) return full;
        }
        return {};
    };

    // 模型1: Stanford Bunny 真实激光扫描
    // 原始坐标范围: X[-33.6, -2.5], Y[-7.4, 23.5], Z[-24.2, 0.0]
    // 中心: (-18, 8, -12) -> 平移到场景左侧 (-5, -8, 5) 使底部落地
    m_modelBunny = new ObjModelRenderer();
    QString bunnyPath = findModel("model_stanford_bunny_scan.obj");
    if (!bunnyPath.isEmpty()) {
        m_modelBunny->load(bunnyPath,
            QVector3D(18.0f, -8.0f, 12.0f),   // 平移：将中心移到场景左侧
            1.0f,
            QVector3D(0.78f, 0.74f, 0.70f));   // 石灰色（真实扫描风格）
        qDebug() << "[Scene] Bunny model loaded:"
                 << m_modelBunny->vertexCount() << "verts,"
                 << m_modelBunny->triangleCount() << "tris";
    } else {
        qWarning() << "[Scene] Bunny model not found, searched:" << searchPaths;
    }

    // 模型2: 摄影测量风格地形网格
    // 原始坐标范围: X[-6.4, 35.7], Y[-0.4, 9.4], Z[-6.6, 26.5]
    // 中心: (14.7, 4.5, 10.0) -> 平移到场景右侧
    m_modelTerrain = new ObjModelRenderer();
    QString terrainPath = findModel("model_photogrammetry_terrain.obj");
    if (!terrainPath.isEmpty()) {
        m_modelTerrain->load(terrainPath,
            QVector3D(-14.7f, -4.5f, -10.0f), // 平移：将中心移到原点附近
            1.0f,
            QVector3D(0.55f, 0.62f, 0.45f));   // 棕绿色（地形风格）
        qDebug() << "[Scene] Terrain model loaded:"
                 << m_modelTerrain->vertexCount() << "verts,"
                 << m_modelTerrain->triangleCount() << "tris";
    } else {
        qWarning() << "[Scene] Terrain model not found, searched:" << searchPaths;
    }
}

void RenderWidget::resizeGL(int w, int h)
{
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, float(w) / float(h ? h : 1), 0.1f, 1000.0f);
}

void RenderWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_view.setToIdentity();
    m_view.translate(0.0f, 0.0f, -m_cameraDistance);
    m_view.rotate(m_cameraPitch, 1.0f, 0.0f, 0.0f);
    m_view.rotate(m_cameraYaw, 0.0f, 1.0f, 0.0f);
    m_view.translate(-5.0f, -8.0f, -5.0f); // 中心点偏移，使两个模型都可见

    m_scene->render(m_projection, m_view);

    // 渲染摄影测量三维模型（在无人机轨迹之前渲染，作为场景背景）
    // 需要切换到固定管线模式
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m_projection.constData());
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m_view.constData());

    if (m_modelBunny)   m_modelBunny->render();
    if (m_modelTerrain) m_modelTerrain->render();

    m_trajRenderer->render(m_projection, m_view);
    m_uavRenderer->render(m_projection, m_view);
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePos = event->position().toPoint();
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = (int)event->position().x() - m_lastMousePos.x();
    int dy = (int)event->position().y() - m_lastMousePos.y();

    if (event->buttons() & Qt::LeftButton) {
        m_cameraYaw += dx * 0.5f;
        m_cameraPitch += dy * 0.5f;
        
        // 限制Pitch
        if (m_cameraPitch < 0.0f) m_cameraPitch = 0.0f;
        if (m_cameraPitch > 90.0f) m_cameraPitch = 90.0f;
        
        update();
    }
    m_lastMousePos = event->position().toPoint();
}

void RenderWidget::wheelEvent(QWheelEvent *event)
{
    m_cameraDistance -= event->angleDelta().y() * 0.05f;
    if (m_cameraDistance < 10.0f) m_cameraDistance = 10.0f;
    if (m_cameraDistance > 500.0f) m_cameraDistance = 500.0f;
    update();
}

void RenderWidget::updateUavState(const QString &uavId, double x, double y, double z, double roll, double pitch, double yaw, double battery)
{
    Q_UNUSED(uavId);
    Q_UNUSED(battery);

    QVector3D pos(x, z, -y); // 简单坐标转换 (假设输入为 SCENE: x右, y前, z上 -> OpenGL: x右, y上, z后)
    m_uavRenderer->updateState(pos, roll, pitch, yaw);
    m_trajRenderer->addPoint(pos);
    update();
}

void RenderWidget::clearTrajectory()
{
    m_trajRenderer->clear();
    update();
}

void RenderWidget::resetView()
{
    m_cameraDistance = 120.0f;
    m_cameraPitch = 45.0f;
    m_cameraYaw = 20.0f;
    update();
}

int RenderWidget::getTrajectoryPointCount() const
{
    return m_trajRenderer->getPointCount();
}
