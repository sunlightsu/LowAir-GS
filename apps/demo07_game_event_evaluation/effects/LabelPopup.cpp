#include "LabelPopup.h"

void LabelPopupQueue::push(const QString& text) {
    // 移除已过期条目
    m_entries.erase(
        std::remove_if(m_entries.begin(), m_entries.end(),
                       [](const LabelPopupEntry& e){ return !e.active; }),
        m_entries.end());

    // 限制最大显示数量
    while (m_entries.size() >= MAX_VISIBLE)
        m_entries.removeFirst();

    m_entries.append(LabelPopupEntry(text, ENTRY_LIFETIME));
}

void LabelPopupQueue::update(float dt) {
    for (auto& e : m_entries) {
        e.elapsed += dt;
        if (e.elapsed >= e.lifetime)
            e.active = false;
    }
    m_entries.erase(
        std::remove_if(m_entries.begin(), m_entries.end(),
                       [](const LabelPopupEntry& e){ return !e.active; }),
        m_entries.end());
}
