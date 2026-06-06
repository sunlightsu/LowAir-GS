#pragma once
#include <QVector3D>
#include <string>

// 双源融合性能与对齐指标
struct FusionMetrics {
    // Mesh 统计
    int   meshVertices  = 0;
    int   meshTriangles = 0;
    QVector3D meshBboxMin;
    QVector3D meshBboxMax;
    QVector3D meshCenter;
    QVector3D meshSize;

    // Gaussian 统计
    int   gaussianCount = 0;
    QVector3D gaussianBboxMin;
    QVector3D gaussianBboxMax;
    QVector3D gaussianCenter;
    QVector3D gaussianSize;

    // 对齐偏差
    QVector3D centerOffset;      // Gaussian 中心 - Mesh 中心（对齐后）
    float     bboxSizeRatioX = 1.0f;
    float     bboxSizeRatioY = 1.0f;
    float     bboxSizeRatioZ = 1.0f;

    // 渲染性能
    float fps         = 0.0f;
    float frameTimeMs = 0.0f;
    std::string displayMode;
    float meshOpacity     = 1.0f;
    float gaussianOpacity = 0.75f;

    void computeAlignment();
};
