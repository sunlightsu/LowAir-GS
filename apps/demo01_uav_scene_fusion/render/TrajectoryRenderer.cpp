#include "TrajectoryRenderer.h"

TrajectoryRenderer::TrajectoryRenderer(QObject *parent)
    : m_program(nullptr)
{
    Q_UNUSED(parent);
}

TrajectoryRenderer::~TrajectoryRenderer()
{
    delete m_program;
}

void TrajectoryRenderer::initialize()
{
    initializeOpenGLFunctions();

    m_program = new QOpenGLShaderProgram();
    
    const char *vsrc =
        "attribute highp vec4 posAttr;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

    const char *fsrc =
        "uniform lowp vec4 color;\n"
        "void main() {\n"
        "   gl_FragColor = color;\n"
        "}\n";

    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
    m_program->link();
}

void TrajectoryRenderer::render(const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
    if (!m_program || m_points.isEmpty()) return;

    m_program->bind();
    m_program->setUniformValue("matrix", projection * view);
    m_program->setUniformValue("color", QVector4D(1.0f, 0.5f, 0.0f, 1.0f)); // 橙色轨迹

    QVector<GLfloat> vertices;
    for (const QVector3D &p : m_points) {
        vertices.append(p.x());
        vertices.append(p.y());
        vertices.append(p.z());
    }

    m_program->enableAttributeArray("posAttr");
    m_program->setAttributeArray("posAttr", vertices.data(), 3);

    glLineWidth(2.0f);
    glDrawArrays(GL_LINE_STRIP, 0, m_points.size());

    m_program->disableAttributeArray("posAttr");
    m_program->release();
}

void TrajectoryRenderer::addPoint(const QVector3D &pos)
{
    m_points.append(pos);
    if (m_points.size() > MAX_POINTS) {
        m_points.removeFirst();
    }
}

void TrajectoryRenderer::clear()
{
    m_points.clear();
}

int TrajectoryRenderer::getPointCount() const
{
    return m_points.size();
}
