#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>

class SimpleScene;
class UavRenderer;
class TrajectoryRenderer;
class ObjModelRenderer;

class RenderWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    RenderWidget(QWidget *parent = nullptr);
    ~RenderWidget();

    void clearTrajectory();
    void resetView();
    int getTrajectoryPointCount() const;

public slots:
    void updateUavState(const QString &uavId, double x, double y, double z, double roll, double pitch, double yaw, double battery);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    SimpleScene *m_scene;
    UavRenderer *m_uavRenderer;
    TrajectoryRenderer *m_trajRenderer;
    ObjModelRenderer *m_modelBunny;    ///< Stanford Bunny 真实激光扫描模型
    ObjModelRenderer *m_modelTerrain;  ///< 摄影测量风格地形网格

    QMatrix4x4 m_projection;
    QMatrix4x4 m_view;

    // Camera control
    float m_cameraDistance;
    float m_cameraPitch;
    float m_cameraYaw;
    QPoint m_lastMousePos;
};

#endif // RENDERWIDGET_H
