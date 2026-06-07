#ifndef TRAJECTORYRENDERER_H
#define TRAJECTORYRENDERER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include <QList>

class TrajectoryRenderer : protected QOpenGLFunctions
{
public:
    TrajectoryRenderer(QObject *parent = nullptr);
    ~TrajectoryRenderer();

    void initialize();
    void render(const QMatrix4x4 &projection, const QMatrix4x4 &view);
    
    void addPoint(const QVector3D &pos);
    void clear();
    int getPointCount() const;

private:
    QOpenGLShaderProgram *m_program;
    QList<QVector3D> m_points;
    const int MAX_POINTS = 1000;
};

#endif // TRAJECTORYRENDERER_H
