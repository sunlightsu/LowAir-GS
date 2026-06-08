#include "FusionMetrics.h"

void FusionMetrics::computeAlignment() {
    meshCenter     = (meshBboxMin + meshBboxMax) * 0.5f;
    gaussianCenter = (gaussianBboxMin + gaussianBboxMax) * 0.5f;
    meshSize       = meshBboxMax - meshBboxMin;
    gaussianSize   = gaussianBboxMax - gaussianBboxMin;
    centerOffset   = gaussianCenter - meshCenter;

    if (meshSize.x() > 0.001f) bboxSizeRatioX = gaussianSize.x() / meshSize.x();
    if (meshSize.y() > 0.001f) bboxSizeRatioY = gaussianSize.y() / meshSize.y();
    if (meshSize.z() > 0.001f) bboxSizeRatioZ = gaussianSize.z() / meshSize.z();
}
