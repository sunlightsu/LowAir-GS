// VirtualExplosionRenderer.cpp
// 虚拟爆炸视觉特效渲染器（非物理、非毁伤模型）
// 仅用于虚拟场景中的视觉反馈，不代表任何真实爆炸、毁伤或武器效果。

#include "VirtualExplosionRenderer.h"
#include <cmath>
#include <cstdlib>
#include <vector>

// ---- 环形/地面圆环 着色器 ----
static const char* RING_VERT = R"(
#version 330 core
layout(location=0) in vec3 aPos;
uniform mat4 uMVP;
uniform vec3 uCenter;
uniform float uRadius;
uniform float uYOffset;
void main(){
    vec3 p = uCenter + vec3(aPos.x * uRadius, uYOffset, aPos.z * uRadius);
    gl_Position = uMVP * vec4(p, 1.0);
}
)";

static const char* RING_FRAG = R"(
#version 330 core
uniform vec4 uColor;
out vec4 fragColor;
void main(){
    fragColor = uColor;
}
)";

// ---- 粒子 着色器 ----
static const char* PART_VERT = R"(
#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec4 aColor;
uniform mat4 uMVP;
out vec4 vColor;
void main(){
    gl_Position = uMVP * vec4(aPos, 1.0);
    gl_PointSize = 4.0;
    vColor = aColor;
}
)";

static const char* PART_FRAG = R"(
#version 330 core
in vec4 vColor;
out vec4 fragColor;
void main(){
    // 圆形点精灵
    vec2 coord = gl_PointCoord - vec2(0.5);
    if (dot(coord, coord) > 0.25) discard;
    fragColor = vColor;
}
)";

VirtualExplosionRenderer::~VirtualExplosionRenderer() { cleanup(); }

void VirtualExplosionRenderer::initialize() {
    initializeOpenGLFunctions();

    // 环形着色器
    m_ringProg.addShaderFromSourceCode(QOpenGLShader::Vertex,   RING_VERT);
    m_ringProg.addShaderFromSourceCode(QOpenGLShader::Fragment, RING_FRAG);
    m_ringProg.link();
    buildRingGeometry();

    // 粒子着色器
    m_partProg.addShaderFromSourceCode(QOpenGLShader::Vertex,   PART_VERT);
    m_partProg.addShaderFromSourceCode(QOpenGLShader::Fragment, PART_FRAG);
    m_partProg.link();

    glGenVertexArrays(1, &m_partVao);
    glGenBuffers(1, &m_partVbo);
    glBindVertexArray(m_partVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_partVbo);
    // pos(3) + color(4) = 7 floats per particle
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    m_initialized = true;
}

void VirtualExplosionRenderer::buildRingGeometry() {
    const int N = 64;
    std::vector<float> verts;
    verts.reserve((N + 1) * 3);
    for (int i = 0; i <= N; ++i) {
        float a = 2.0f * 3.14159265f * i / N;
        verts.push_back(cosf(a));
        verts.push_back(0.0f);
        verts.push_back(sinf(a));
    }
    m_ringVertexCount = (int)verts.size() / 3;

    glGenVertexArrays(1, &m_ringVao);
    glGenBuffers(1, &m_ringVbo);
    glBindVertexArray(m_ringVao);
    glBindBuffer(GL_ARRAY_BUFFER, m_ringVbo);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

static float randF(float lo, float hi) {
    return lo + (hi - lo) * ((float)rand() / RAND_MAX);
}

void VirtualExplosionRenderer::spawnExplosion(const QVector3D& center) {
    // 添加特效实例
    VirtualExplosionEffect fx;
    fx.position = center;
    m_effects.push_back(fx);

    // 橙红色粒子喷发（80 个）
    for (int i = 0; i < 80; ++i) {
        ExplosionParticle p;
        p.pos = center;
        float theta = randF(0.0f, 2.0f * 3.14159265f);
        float phi   = randF(0.1f, 1.2f);  // 偏向上方喷发
        float speed = randF(3.0f, 9.0f);
        p.vel = QVector3D(
            cosf(theta) * sinf(phi) * speed,
            cosf(phi) * speed,
            sinf(theta) * sinf(phi) * speed
        );
        // 橙红色（随机在橙色到红色之间）
        float r = randF(0.85f, 1.0f);
        float g = randF(0.2f, 0.55f);
        p.color = QVector4D(r, g, 0.0f, 1.0f);
        p.maxLife = randF(0.8f, 1.8f);
        p.life    = p.maxLife;
        m_particles.push_back(p);
    }

    // 烟雾状半透明粒子（40 个）
    for (int i = 0; i < 40; ++i) {
        ExplosionParticle p;
        p.pos = center + QVector3D(randF(-1.5f, 1.5f), randF(0.0f, 1.0f), randF(-1.5f, 1.5f));
        float theta = randF(0.0f, 2.0f * 3.14159265f);
        float speed = randF(0.5f, 2.5f);
        p.vel = QVector3D(cosf(theta) * speed, randF(0.5f, 2.0f), sinf(theta) * speed);
        // 灰白色半透明烟雾
        float g = randF(0.6f, 0.85f);
        p.color = QVector4D(g, g, g, randF(0.3f, 0.6f));
        p.maxLife = randF(1.5f, 2.5f);
        p.life    = p.maxLife;
        m_particles.push_back(p);
    }
}

void VirtualExplosionRenderer::update(float dt) {
    // 更新特效生命周期
    for (auto& fx : m_effects) {
        fx.elapsed += dt;
        if (fx.elapsed >= fx.lifetime) fx.active = false;
    }
    m_effects.erase(
        std::remove_if(m_effects.begin(), m_effects.end(),
                       [](const VirtualExplosionEffect& f){ return !f.active; }),
        m_effects.end());

    // 更新粒子（重力 -4.0 m/s²，模拟轻微上升后下落）
    for (auto& p : m_particles) {
        p.vel.setY(p.vel.y() - 4.0f * dt);
        p.pos += p.vel * dt;
        p.life -= dt;
        // 随时间淡出
        float t = 1.0f - std::max(0.0f, p.life / p.maxLife);
        p.color.setW(p.color.w() * (1.0f - t * 0.03f));
    }
    m_particles.erase(
        std::remove_if(m_particles.begin(), m_particles.end(),
                       [](const ExplosionParticle& p){ return p.life <= 0.0f; }),
        m_particles.end());
}

void VirtualExplosionRenderer::render(const QMatrix4x4& mvp) {
    if (!m_initialized) return;

    // ---- 渲染环形波纹和地面圆环 ----
    m_ringProg.bind();
    m_ringProg.setUniformValue("uMVP", mvp);
    glBindVertexArray(m_ringVao);
    glLineWidth(2.0f);

    for (const auto& fx : m_effects) {
        if (!fx.active) continue;

        // 1. 中心闪光（小半径白色环，快速衰减）
        float fa = fx.flashAlpha();
        if (fa > 0.0f) {
            m_ringProg.setUniformValue("uCenter",  fx.position);
            m_ringProg.setUniformValue("uRadius",  fx.flashRadius * (0.5f + fx.progress() * 0.5f));
            m_ringProg.setUniformValue("uYOffset", 0.0f);
            m_ringProg.setUniformValue("uColor",   QVector4D(1.0f, 0.95f, 0.7f, fa));
            glDrawArrays(GL_LINE_STRIP, 0, m_ringVertexCount);
            // 第二层橙色环
            m_ringProg.setUniformValue("uRadius",  fx.flashRadius * (0.8f + fx.progress() * 0.5f));
            m_ringProg.setUniformValue("uColor",   QVector4D(1.0f, 0.5f, 0.1f, fa * 0.7f));
            glDrawArrays(GL_LINE_STRIP, 0, m_ringVertexCount);
        }

        // 2. 向外扩散的环形波纹（3 层，仅视觉）
        float wa = fx.waveAlpha();
        if (wa > 0.0f) {
            float wr = fx.waveRadius();
            for (int layer = 0; layer < 3; ++layer) {
                float layerR = wr * (1.0f + layer * 0.15f);
                float layerA = wa * (1.0f - layer * 0.25f);
                if (layerA <= 0.0f) continue;
                m_ringProg.setUniformValue("uCenter",  fx.position);
                m_ringProg.setUniformValue("uRadius",  layerR);
                m_ringProg.setUniformValue("uYOffset", 0.0f);
                // 橙红渐变
                float rr = 1.0f;
                float rg = 0.3f + (1.0f - layerA) * 0.3f;
                m_ringProg.setUniformValue("uColor", QVector4D(rr, rg, 0.0f, layerA));
                glDrawArrays(GL_LINE_STRIP, 0, m_ringVertexCount);
            }
        }

        // 3. 地面短时发光圆环（Y=position.y，橙黄色）
        float ga = fx.groundAlpha();
        if (ga > 0.0f) {
            // 地面圆环（在目标点所在高度）
            m_ringProg.setUniformValue("uCenter",  fx.position);
            m_ringProg.setUniformValue("uRadius",  fx.groundRadius);
            m_ringProg.setUniformValue("uYOffset", 0.0f);
            m_ringProg.setUniformValue("uColor",   QVector4D(1.0f, 0.7f, 0.0f, ga * 0.8f));
            glDrawArrays(GL_LINE_STRIP, 0, m_ringVertexCount);
            // 内层发光环
            m_ringProg.setUniformValue("uRadius",  fx.groundRadius * 0.6f);
            m_ringProg.setUniformValue("uColor",   QVector4D(1.0f, 0.9f, 0.3f, ga * 0.5f));
            glDrawArrays(GL_LINE_STRIP, 0, m_ringVertexCount);
        }
    }

    glLineWidth(1.0f);
    glBindVertexArray(0);
    m_ringProg.release();

    // ---- 渲染粒子（橙红 + 烟雾）----
    if (!m_particles.empty()) {
        // 上传粒子数据
        std::vector<float> data;
        data.reserve(m_particles.size() * 7);
        for (const auto& p : m_particles) {
            if (p.life <= 0.0f) continue;
            float alpha = std::max(0.0f, p.life / p.maxLife);
            data.push_back(p.pos.x());
            data.push_back(p.pos.y());
            data.push_back(p.pos.z());
            data.push_back(p.color.x());
            data.push_back(p.color.y());
            data.push_back(p.color.z());
            data.push_back(p.color.w() * alpha);
        }

        glBindVertexArray(m_partVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_partVbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);

        m_partProg.bind();
        m_partProg.setUniformValue("uMVP", mvp);
        glEnable(GL_PROGRAM_POINT_SIZE);
        glDrawArrays(GL_POINTS, 0, (GLsizei)(data.size() / 7));
        glDisable(GL_PROGRAM_POINT_SIZE);
        m_partProg.release();
        glBindVertexArray(0);
    }
}

void VirtualExplosionRenderer::cleanup() {
    if (m_ringVao) { glDeleteVertexArrays(1, &m_ringVao); m_ringVao = 0; }
    if (m_ringVbo) { glDeleteBuffers(1, &m_ringVbo);      m_ringVbo = 0; }
    if (m_partVao) { glDeleteVertexArrays(1, &m_partVao); m_partVao = 0; }
    if (m_partVbo) { glDeleteBuffers(1, &m_partVbo);      m_partVbo = 0; }
}
