/**
 * OrbitCamera.cpp — 轨道相机实现
 */

#include "OrbitCamera.h"
#include <QtMath>
#include <algorithm>

OrbitCamera::OrbitCamera()
    : m_target(0.0f, 0.0f, 0.0f)
    , m_radius(80.0f)
    , m_azimuth(45.0f)
    , m_elevation(30.0f)
    , m_fov(45.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(5000.0f)
{}

void OrbitCamera::setTarget(const QVector3D &t)    { m_target    = t; }
void OrbitCamera::setRadius(float r)               { m_radius    = std::max(0.5f, r); }
void OrbitCamera::setAzimuth(float deg)            { m_azimuth   = deg; }
void OrbitCamera::setElevation(float deg)          { m_elevation = std::clamp(deg, 5.0f, 89.0f); }
void OrbitCamera::setFov(float deg)                { m_fov       = std::clamp(deg, 10.0f, 120.0f); }

void OrbitCamera::orbit(float dAz, float dEl)
{
    m_azimuth   += dAz;
    m_elevation  = std::clamp(m_elevation + dEl, 5.0f, 89.0f);
}

void OrbitCamera::pan(float dx, float dy)
{
    // 在相机的右向量和上向量方向平移目标点
    QVector3D pos = position();
    QVector3D forward = (m_target - pos).normalized();
    QVector3D right   = QVector3D::crossProduct(forward, QVector3D(0,0,1)).normalized();
    QVector3D up      = QVector3D::crossProduct(right, forward).normalized();
    float scale = m_radius * 0.001f;
    m_target -= right * dx * scale;
    m_target += up    * dy * scale;
}

void OrbitCamera::zoom(float delta)
{
    m_radius = std::max(0.5f, m_radius - delta * m_radius * 0.1f);
}

void OrbitCamera::resetToDefault()
{
    m_target    = QVector3D(0.0f, 0.0f, 0.0f);
    m_radius    = 80.0f;
    m_azimuth   = 45.0f;
    m_elevation = 30.0f;
}

void OrbitCamera::fitToBox(float x0, float y0, float z0,
                            float x1, float y1, float z1)
{
    // 目标点设为包围盒中心
    m_target = QVector3D((x0+x1)*0.5f, (y0+y1)*0.5f, (z0+z1)*0.5f);

    // 包围盒对角线长度的 1.5 倍作为相机距离
    float dx = x1 - x0, dy = y1 - y0, dz = z1 - z0;
    float diag = std::sqrt(dx*dx + dy*dy + dz*dz);
    m_radius = diag * 1.5f;
    if (m_radius < 1.0f) m_radius = 1.0f;

    m_azimuth   = 45.0f;
    m_elevation = 30.0f;
}

QVector3D OrbitCamera::position() const
{
    float az = qDegreesToRadians(m_azimuth);
    float el = qDegreesToRadians(m_elevation);
    float cosEl = std::cos(el);
    return m_target + QVector3D(
        m_radius * cosEl * std::sin(az),
        m_radius * cosEl * std::cos(az),
        m_radius * std::sin(el)
    );
}

QMatrix4x4 OrbitCamera::viewMatrix() const
{
    QMatrix4x4 mat;
    mat.lookAt(position(), m_target, QVector3D(0.0f, 0.0f, 1.0f));
    return mat;
}

QMatrix4x4 OrbitCamera::projectionMatrix(float aspect) const
{
    QMatrix4x4 mat;
    mat.perspective(m_fov, aspect, m_nearPlane, m_farPlane);
    return mat;
}
