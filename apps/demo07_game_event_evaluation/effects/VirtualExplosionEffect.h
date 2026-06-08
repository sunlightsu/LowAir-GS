#pragma once
// VirtualExplosionEffect.h
// 虚拟爆炸视觉特效（非物理、非毁伤模型）
// 仅用于虚拟场景中的视觉反馈，不代表任何真实爆炸、毁伤或武器效果。

#include <QVector3D>
#include <QVector4D>

// 虚拟爆炸视觉特效实例
// 包含：中心闪光、环形波纹、橙红粒子、烟雾粒子、地面发光圆环
struct VirtualExplosionEffect {
    QVector3D position;           // 爆发中心位置
    float     lifetime  = 2.5f;  // 总生命周期（秒）
    float     elapsed   = 0.0f;  // 已经过时间（秒）
    bool      active    = true;

    // 各阶段参数
    float flashRadius   = 3.0f;  // 中心闪光半径
    float waveMaxRadius = 12.0f; // 环形波纹最大扩散半径
    float groundRadius  = 8.0f;  // 地面发光圆环半径

    float progress() const {
        return (lifetime > 0.0f) ? (elapsed / lifetime) : 1.0f;
    }

    // 中心闪光 alpha（前 20% 快速衰减）
    float flashAlpha() const {
        float t = progress();
        if (t > 0.3f) return 0.0f;
        return 1.0f - (t / 0.3f);
    }

    // 环形波纹 alpha（前 70% 线性衰减）
    float waveAlpha() const {
        float t = progress();
        if (t > 0.8f) return 0.0f;
        return 1.0f - (t / 0.8f);
    }

    // 环形波纹当前半径（随时间扩散）
    float waveRadius() const {
        float t = progress();
        return flashRadius * 0.5f + waveMaxRadius * std::min(t * 1.5f, 1.0f);
    }

    // 地面圆环 alpha（中段持续，两端衰减）
    float groundAlpha() const {
        float t = progress();
        if (t < 0.1f) return t / 0.1f;
        if (t > 0.7f) return 1.0f - (t - 0.7f) / 0.3f;
        return 1.0f;
    }
};
