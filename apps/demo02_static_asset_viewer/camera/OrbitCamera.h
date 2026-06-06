/**
 * OrbitCamera.h — 轨道相机（Orbit Camera）
 *
 * 实现围绕目标点旋转的相机控制：
 *   - 左键拖拽：旋转（方位角 azimuth + 仰角 elevation）
 *   - 右键拖拽：平移目标点（pan）
 *   - 滚轮：缩放（zoom）
 *
 * 球坐标系：
 *   x = target.x + radius * cos(elevation) * sin(azimuth)
 *   y = target.y + radius * cos(elevation) * cos(azimuth)
 *   z = target.z + radius * sin(elevation)
 */

#pragma once

#include <QMatrix4x4>
#include <QVector3D>

class OrbitCamera
{
public:
    OrbitCamera();

    // ── 参数设置 ──────────────────────────────────────────────────────────
    void setTarget(const QVector3D &target);
    void setRadius(float radius);
    void setAzimuth(float deg);
    void setElevation(float deg);
    void setFov(float deg);

    // ── 交互操作 ──────────────────────────────────────────────────────────
    void orbit(float dAzimuth, float dElevation);   // 旋转（度）
    void pan(float dx, float dy);                    // 平移（世界单位）
    void zoom(float delta);                          // 缩放（正=拉近）

    // ── 预设视角 ──────────────────────────────────────────────────────────
    void resetToDefault();
    void fitToBox(float minX, float minY, float minZ,
                  float maxX, float maxY, float maxZ);

    // ── 矩阵获取 ──────────────────────────────────────────────────────────
    QMatrix4x4 viewMatrix() const;
    QMatrix4x4 projectionMatrix(float aspectRatio) const;
    QVector3D  position() const;

    // ── 属性读取 ──────────────────────────────────────────────────────────
    float radius()    const { return m_radius; }
    float azimuth()   const { return m_azimuth; }
    float elevation() const { return m_elevation; }
    QVector3D target() const { return m_target; }

private:
    QVector3D m_target;
    float m_radius;
    float m_azimuth;    // 水平旋转角（度）
    float m_elevation;  // 仰角（度），限制在 [5, 89]
    float m_fov;        // 视场角（度）
    float m_nearPlane;
    float m_farPlane;
};
