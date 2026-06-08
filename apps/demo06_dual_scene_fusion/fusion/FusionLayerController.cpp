#include "FusionLayerController.h"

FusionLayerController::FusionLayerController() {}

bool FusionLayerController::isMeshVisible() const {
    if (!m_meshEnabled) return false;
    switch (m_displayMode) {
        case FusionDisplayMode::GaussianOnly: return false;
        default: return true;
    }
}

bool FusionLayerController::isGaussianVisible() const {
    if (!m_gaussianEnabled) return false;
    switch (m_displayMode) {
        case FusionDisplayMode::MeshOnly: return false;
        default: return true;
    }
}

bool FusionLayerController::isMeshWireframeOverlay() const {
    return m_displayMode == FusionDisplayMode::WireframeComparison;
}

std::string FusionLayerController::displayModeName() const {
    switch (m_displayMode) {
        case FusionDisplayMode::MeshOnly:            return "Mesh Only";
        case FusionDisplayMode::GaussianOnly:        return "Gaussian Only";
        case FusionDisplayMode::DualFusion:          return "Dual Fusion";
        case FusionDisplayMode::WireframeComparison: return "Wireframe Comparison";
    }
    return "Unknown";
}
