#pragma once
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QVector3D>
#include <vector>

// 轨迹渲染器：
//   - 普通历史轨迹（青色）
//   - 已完成任务段高亮（亮黄色）
// Path Highlight 是任务完成过程的视觉提示，不代表真实航线优化。
class TrajectoryRenderer : public QOpenGLExtraFunctions {
public:
    static constexpr int MAX_POINTS = 3000;

    TrajectoryRenderer() = default;
    ~TrajectoryRenderer();

    void initialize();
    void addPoint(const QVector3D& p);
    void render(const QMatrix4x4& mvp);
    void clear();
    void cleanup();
    int  pointCount() const { return (int)m_points.size(); }

    // 设置高亮截止索引：0 ~ pointCount()-1 之前的点用高亮颜色绘制
    void setHighlightUpTo(int idx) { m_highlightUpTo = idx; }
    int  highlightUpTo() const     { return m_highlightUpTo; }

private:
    QOpenGLShaderProgram m_prog;
    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    std::vector<QVector3D> m_points;
    bool m_initialized = false;
    bool m_dirty = false;
    int  m_highlightUpTo = 0;  // 高亮到第几个点
};
