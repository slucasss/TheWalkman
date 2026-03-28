#include "CircleCollider.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Debug.h"
#include "imgui.h"

#define M_PI 3.14159265358979323846

bool CircleCollider::IsColliding(Collider* _target)
{
    if (m_isActived == false || _target->IsActived() == false)
        return false;

    if (_target->GetType() == ColliderType::Circle)
    {
        CircleCollider* c = (CircleCollider*)_target;

        sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
        sf::Vector2f targetPos = c->GetOwner()->GetComponent<TransformComponent>()->GetPosition();

        float dx = targetPos.x - pos.x;
        float dy = targetPos.y - pos.y;
        float r = m_radius + c->m_radius;

        return dx * dx + dy * dy <= r * r;
    }
    else
    {
        return _target->IsColliding(this);
    }
}

void CircleCollider::Draw()
{
    if (m_isVisible == false)
        return;

    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    pos = { pos.x + m_offsetX, pos.y + m_offsetY };

    Debug::DrawCircle(pos.x, pos.y, m_radius, sf::Color::Green);
}

void CircleCollider::SetRadius(float _radius)
{
    m_radius = _radius;
}

float CircleCollider::GetLeft()
{
    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    return pos.x + m_offsetX - m_radius;
}

float CircleCollider::GetRight()
{
    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    return pos.x + m_offsetX + m_radius;
}

float CircleCollider::GetTop()
{
    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    return pos.y + m_offsetY - m_radius;
}

float CircleCollider::GetBottom()
{
    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    return pos.y + m_offsetY + m_radius;
}

void CircleCollider::Inspect()
{
    ImGui::DragFloat("Radius", &m_radius);

    Collider::Inspect();
}

