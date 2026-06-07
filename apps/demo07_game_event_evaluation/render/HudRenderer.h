#pragma once
#include <QPainter>
#include <QWidget>
#include <QString>
#include "../effects/LabelPopup.h"

// HUD 叠加渲染器（使用 QPainter 在 OpenGL 视口上绘制文字）
class HudRenderer {
public:
    HudRenderer() = default;

    // 渲染主 HUD 状态栏
    void render(QPainter& painter, int viewW, int viewH,
                int score, int maxScore,
                float elapsedSec, float battery,
                int events, const QString& missionStatus);

    // 渲染 Label Popup 弹窗（右上角）
    void renderPopups(QPainter& painter, int viewW, int viewH,
                      const LabelPopupQueue& popups);
};
