#pragma once
#include <QMatrix4x4>
#include <QVector3D>

// 双源对齐变换（Gaussian 相对 Mesh 的 TRS 变换）
class FusionTransform {
public:
    FusionTransform();

    void setTranslation(const QVector3D& t);
    void setRotationDeg(const QVector3D& rDeg);  // roll/pitch/yaw in degrees
    void setScale(float s);

    QVector3D translation() const { return m_translation; }
    QVector3D rotationDeg() const { return m_rotationDeg; }
    float     scale()       const { return m_scale; }

    // 计算 Gaussian 相对 Mesh 的模型矩阵
    QMatrix4x4 gaussianModelMatrix() const;

    // 一键重置为恒等变换
    void reset();

    // 中心点对齐：将 Gaussian 中心移动到 Mesh 中心
    void centerAlign(const QVector3D& meshCenter, const QVector3D& gaussianCenter);

    // 包围盒尺度归一：将 Gaussian 缩放到与 Mesh 包围盒相同大小
    void bboxScaleMatch(const QVector3D& meshSize, const QVector3D& gaussianSize);

private:
    QVector3D m_translation = {0, 0, 0};
    QVector3D m_rotationDeg = {0, 0, 0};
    float     m_scale       = 1.0f;
};
