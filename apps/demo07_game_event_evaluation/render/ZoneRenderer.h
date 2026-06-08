#pragma once
#include "../mission/TriggerZone.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <vector>

// 触发区域渲染器：
//   - 未触发：半透明蓝色线框
//   - 触发中：亮蓝色脉冲呼吸动画线框
//   - 已完成：绿色线框
class ZoneRenderer : public QOpenGLExtraFunctions {
public:
    ZoneRenderer() = default;
    ~ZoneRenderer();

    void initialize();
    void render(const QMatrix4x4& mvp, const std::vector<TriggerZone>& zones);
    void cleanup();

private:
    void buildBoxGeometry();

    QOpenGLShaderProgram m_prog;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ibo = 0;
    int    m_indexCount = 0;
    bool   m_initialized = false;
    float  m_pulseTime = 0.0f;  // 脉冲动画时间累计
};
