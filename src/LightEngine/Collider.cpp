#include "Collider.h"
#include "imgui.h"

Collider::Collider(ColliderType _type, uint32_t _layer, uint32_t _mask)
{
    m_type = _type;
    m_offsetX = 0;
    m_offsetY = 0;

    m_isActived = true;
    m_isVisible = true;

    m_layer = _layer;
    m_mask = _mask;
}

bool Collider::CanCollide(Collider* _other)
{
    return (m_mask & _other->m_layer) != 0 && (_other->m_mask & m_layer) != 0;
}

void Collider::SetOffset(float _ox, float _oy)
{
    m_offsetX = _ox;
    m_offsetY = _oy;
}

void Collider::Inspect()
{
    sf::Vector2f offset = { m_offsetX, m_offsetY };

    if(ImGui::DragFloat2("Offset", &offset.x, 0.1f))
        SetOffset(offset.x, offset.y);

    ImGui::Checkbox("Actived", &m_isActived);
    ImGui::Checkbox("Trigger", &m_isTrigger);
    ImGui::Checkbox("Visible", &m_isVisible);
}
