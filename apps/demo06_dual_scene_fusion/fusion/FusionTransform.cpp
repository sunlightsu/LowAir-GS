#include "FusionTransform.h"
#include <cmath>
#include <algorithm>

FusionTransform::FusionTransform() {}

void FusionTransform::setTranslation(const QVector3D& t) { m_translation = t; }
void FusionTransform::setRotationDeg(const QVector3D& r) { m_rotationDeg = r; }
void FusionTransform::setScale(float s) { m_scale = s > 0.0001f ? s : 0.0001f; }

QMatrix4x4 FusionTransform::gaussianModelMatrix() const {
    QMatrix4x4 m;
    m.setToIdentity();
    m.translate(m_translation);
    m.rotate(m_rotationDeg.z(), 0, 0, 1);  // yaw
    m.rotate(m_rotationDeg.y(), 0, 1, 0);  // pitch
    m.rotate(m_rotationDeg.x(), 1, 0, 0);  // roll
    m.scale(m_scale);
    return m;
}

void FusionTransform::reset() {
    m_translation = {0, 0, 0};
    m_rotationDeg = {0, 0, 0};
    m_scale       = 1.0f;
}

void FusionTransform::centerAlign(const QVector3D& meshCenter,
                                  const QVector3D& gaussianCenter) {
    // 将 Gaussian 中心平移到 Mesh 中心
    m_translation = meshCenter - gaussianCenter * m_scale;
}

void FusionTransform::bboxScaleMatch(const QVector3D& meshSize,
                                     const QVector3D& gaussianSize) {
    float mMax = std::max({meshSize.x(), meshSize.y(), meshSize.z()});
    float gMax = std::max({gaussianSize.x(), gaussianSize.y(), gaussianSize.z()});
    if (gMax > 0.0001f) {
        m_scale = mMax / gMax;
    }
}
