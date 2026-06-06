#include "GaussianBillboardRenderer.h"
#include <algorithm>
#include <cmath>

GaussianBillboardRenderer::GaussianBillboardRenderer() {}
GaussianBillboardRenderer::~GaussianBillboardRenderer() { cleanup(); }

void GaussianBillboardRenderer::buildShaders() {
    // Point shader
    const char* pvs = R"(
        #version 330 core
        layout(location=0) in vec3 aPos;
        layout(location=1) in vec3 aColor;
        layout(location=2) in float aOpacity;
        layout(location=3) in vec3 aScale;
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;
        uniform float uGlobalOpacity;
        out vec4 vColor;
        void main() {
            gl_Position  = uProj * uView * uModel * vec4(aPos, 1.0);
            float sz     = max(aScale.x, max(aScale.y, aScale.z));
            gl_PointSize = clamp(sz * 80.0, 2.0, 20.0);
            vColor = vec4(aColor, aOpacity * uGlobalOpacity);
        }
    )";
    const char* pfs = R"(
        #version 330 core
        in vec4 vColor;
        out vec4 fragColor;
        void main() {
            vec2 c = gl_PointCoord - 0.5;
            if (dot(c,c) > 0.25) discard;
            fragColor = vColor;
        }
    )";
    m_pointProg.addShaderFromSourceCode(QOpenGLShader::Vertex,   pvs);
    m_pointProg.addShaderFromSourceCode(QOpenGLShader::Fragment, pfs);
    m_pointProg.link();

    // Splat (billboard) shader
    const char* svs = R"(
        #version 330 core
        layout(location=0) in vec3 aPos;
        layout(location=1) in vec3 aColor;
        layout(location=2) in float aOpacity;
        layout(location=3) in vec3 aScale;
        uniform mat4 uModel;
        uniform mat4 uView;
        uniform mat4 uProj;
        uniform float uGlobalOpacity;
        out vec4 vColor;
        void main() {
            gl_Position  = uProj * uView * uModel * vec4(aPos, 1.0);
            float sz     = max(aScale.x, max(aScale.y, aScale.z));
            gl_PointSize = clamp(sz * 160.0, 4.0, 40.0);
            vColor = vec4(aColor, aOpacity * uGlobalOpacity);
        }
    )";
    const char* sfs = R"(
        #version 330 core
        in vec4 vColor;
        out vec4 fragColor;
        void main() {
            vec2 c = gl_PointCoord - 0.5;
            float r2 = dot(c, c);
            if (r2 > 0.25) discard;
            float alpha = exp(-8.0 * r2) * vColor.a;
            fragColor = vec4(vColor.rgb, alpha);
        }
    )";
    m_splatProg.addShaderFromSourceCode(QOpenGLShader::Vertex,   svs);
    m_splatProg.addShaderFromSourceCode(QOpenGLShader::Fragment, sfs);
    m_splatProg.link();
}

void GaussianBillboardRenderer::initialize() {
    if (m_initialized) return;
    initializeOpenGLFunctions();
    buildShaders();
    m_initialized = true;
}

void GaussianBillboardRenderer::upload(const GaussianCloud& cloud) {
    if (!m_initialized) return;
    releaseGPU();

    const auto& pts = cloud.points();
    m_count = static_cast<int>(pts.size());
    if (m_count == 0) return;

    // VBO 布局: pos(3) + color(3) + opacity(1) + scale(3) = 10 floats
    std::vector<float> buf;
    buf.reserve(m_count * 10);
    for (const auto& p : pts) {
        buf.push_back(p.position.x()); buf.push_back(p.position.y()); buf.push_back(p.position.z());
        buf.push_back(p.color.x());    buf.push_back(p.color.y());    buf.push_back(p.color.z());
        buf.push_back(p.opacity);
        buf.push_back(p.scale.x()); buf.push_back(p.scale.y()); buf.push_back(p.scale.z());
    }

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, buf.size() * sizeof(float), buf.data(), GL_STATIC_DRAW);

    GLsizei stride = 10 * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(7*sizeof(float)));
    glBindVertexArray(0);
    m_uploaded = true;
}

void GaussianBillboardRenderer::render(const QMatrix4x4& model,
                                       const QMatrix4x4& view,
                                       const QMatrix4x4& proj,
                                       GaussianRenderMode mode,
                                       float              opacity) {
    if (!m_initialized || !m_uploaded || m_count == 0) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    QOpenGLShaderProgram* prog = (mode == GaussianRenderMode::Splat) ? &m_splatProg : &m_pointProg;
    prog->bind();
    prog->setUniformValue("uModel",         model);
    prog->setUniformValue("uView",          view);
    prog->setUniformValue("uProj",          proj);
    prog->setUniformValue("uGlobalOpacity", opacity);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_POINTS, 0, m_count);
    glBindVertexArray(0);
    prog->release();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_PROGRAM_POINT_SIZE);
}

void GaussianBillboardRenderer::releaseGPU() {
    if (m_vao) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
    if (m_vbo) { glDeleteBuffers(1, &m_vbo);      m_vbo = 0; }
    m_count    = 0;
    m_uploaded = false;
}

void GaussianBillboardRenderer::cleanup() {
    if (m_initialized) {
        releaseGPU();
        m_initialized = false;
    }
}
