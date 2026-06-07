#ifndef UAVRENDERER_H
#define UAVRENDERER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>

class UavRenderer : protected QOpenGLFunctions
{
public:
    UavRenderer(QObject *parent = nullptr);
    ~UavRenderer();

    void initialize();
    void render(const QMatrix4x4 &projection, const QMatrix4x4 &view);
    void updateState(const QVector3D &pos, double roll, double pitch, double yaw);

private:
    QOpenGLShaderProgram *m_program;
    QVector3D m_position;
    double m_roll;
    double m_pitch;
    double m_yaw;
};

#endif // UAVRENDERER_H
