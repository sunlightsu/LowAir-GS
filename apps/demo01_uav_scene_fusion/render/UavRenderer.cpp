#include "UavRenderer.h"
#include <QDebug>

UavRenderer::UavRenderer(QObject *parent)
    : m_program(nullptr)
    , m_position(0, 0, 0)
    , m_roll(0), m_pitch(0), m_yaw(0)
{
    Q_UNUSED(parent);
}

UavRenderer::~UavRenderer()
{
    delete m_program;
}

void UavRenderer::initialize()
{
    initializeOpenGLFunctions();

    m_program = new QOpenGLShaderProgram();
    
    const char *vsrc =
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

    const char *fsrc =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";

    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vsrc);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fsrc);
    m_program->link();
}

void UavRenderer::render(const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
    if (!m_program) return;

    m_program->bind();

    QMatrix4x4 model;
    model.translate(m_position);
    model.rotate(m_yaw, 0.0f, 1.0f, 0.0f);
    model.rotate(m_pitch, 1.0f, 0.0f, 0.0f);
    model.rotate(m_roll, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 matrix = projection * view * model;
    m_program->setUniformValue("matrix", matrix);

    // 简单无人机模型 (一个向前的箭头)
    GLfloat vertices[] = {
        // 主体
        0.0f, 0.0f, -2.0f,
        -1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f
    };

    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f, 1.0f, // 红头
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 1, 3,
        0, 2, 3,
        1, 2, 3
    };

    m_program->enableAttributeArray("posAttr");
    m_program->enableAttributeArray("colAttr");
    m_program->setAttributeArray("posAttr", vertices, 3);
    m_program->setAttributeArray("colAttr", colors, 4);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, indices);

    m_program->disableAttributeArray("posAttr");
    m_program->disableAttributeArray("colAttr");

    m_program->release();
}

void UavRenderer::updateState(const QVector3D &pos, double roll, double pitch, double yaw)
{
    m_position = pos;
    m_roll = roll;
    m_pitch = pitch;
    m_yaw = yaw;
}
