#pragma once
#include <QString>
#include <QVector>

// 单条文字弹窗条目
struct LabelPopupEntry {
    QString text;          // 显示文字，如 "+20 Target Reached"
    float   lifetime;      // 总显示时长（秒）
    float   elapsed;       // 已经过时间（秒）
    bool    active;

    LabelPopupEntry(const QString& t, float life = 2.5f)
        : text(t), lifetime(life), elapsed(0.0f), active(true) {}

    // 0.0 ~ 1.0，用于淡出
    float alpha() const {
        if (!active) return 0.0f;
        float t = elapsed / lifetime;
        // 前 30% 淡入，后 40% 淡出
        if (t < 0.3f) return t / 0.3f;
        if (t > 0.6f) return 1.0f - (t - 0.6f) / 0.4f;
        return 1.0f;
    }
};

// 弹窗队列管理器
class LabelPopupQueue {
public:
    static constexpr int MAX_VISIBLE = 5;   // 最多同时显示 5 条
    static constexpr float ENTRY_LIFETIME = 2.5f;

    void push(const QString& text);
    void update(float dt);
    const QVector<LabelPopupEntry>& entries() const { return m_entries; }

private:
    QVector<LabelPopupEntry> m_entries;
};
