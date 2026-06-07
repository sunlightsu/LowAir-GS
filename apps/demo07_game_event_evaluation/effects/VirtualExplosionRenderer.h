#pragma once
// VirtualExplosionRenderer.h
// 虚拟爆炸视觉特效渲染器（非物理、非毁伤模型）
// 仅用于虚拟场景中的视觉反馈，不代表任何真实爆炸、毁伤或武器效果。

#include "VirtualExplosionEffect.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <vector>

struct ExplosionParticle {
    QVector3D pos;
    QVector3D vel;
    QVector4D color;   // RGBA
    float     life    = 1.0f;
    float     maxLife = 1.0f;
};

// 虚拟爆炸视觉特效渲染器
// 渲染：中心闪光、环形波纹、橙红粒子、烟雾粒子、地面发光圆环
class VirtualExplosionRenderer : public QOpenGLExtraFunctions {
public:
    VirtualExplosionRenderer() = default;
    ~VirtualExplosionRenderer();

    void initialize();

    // 触发一次虚拟爆炸视觉特效（非物理、非毁伤）
    void spawnExplosion(const QVector3D& center);

    // 更新粒子和特效状态
    void update(float dt);

    // 渲染所有活跃特效
    void render(const QMatrix4x4& mvp);

    void cleanup();

    bool hasActiveEffects() const {
        return !m_effects.empty() || !m_particles.empty();
    }

private:
    void buildRingGeometry();
    void buildQuadGeometry();

    // 环形/地面圆环渲染
    QOpenGLShaderProgram m_ringProg;
    GLuint m_ringVao = 0;
    GLuint m_ringVbo = 0;
    int    m_ringVertexCount = 0;

    // 粒子渲染（橙红 + 烟雾）
    QOpenGLShaderProgram m_partProg;
    GLuint m_partVao = 0;
    GLuint m_partVbo = 0;

    std::vector<VirtualExplosionEffect> m_effects;
    std::vector<ExplosionParticle>      m_particles;

    bool m_initialized = false;
};
