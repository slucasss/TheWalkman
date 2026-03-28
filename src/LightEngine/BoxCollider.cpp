#include "BoxCollider.h"
#include "CircleCollider.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Debug.h"
#include "imgui.h"

bool BoxCollider::IsColliding(Collider* other)
{
    if (m_isActived == false || other->IsActived() == false)
        return false;

    if (other->GetType() == ColliderType::Rectangle)
    {
        BoxCollider* r = (BoxCollider*)other;

        return !(GetRight() < r->GetLeft() ||
            GetLeft() > r->GetRight() ||
            GetBottom() < r->GetTop() ||
            GetTop() > r->GetBottom());
    }
    else // Circle
    {
        CircleCollider* c = (CircleCollider*)other;

        sf::Vector2f pos = c->GetOwner()->GetComponent<TransformComponent>()->GetPosition();
        float radius = c->GetRadius();

        // clamp du point du cercle dans le rectangle
        float closestX = std::max(GetLeft(), std::min(pos.x, GetRight()));
        float closestY = std::max(GetTop(), std::min(pos.y, GetBottom()));

        float dx = closestX - pos.x;
        float dy = closestY - pos.y;

        return dx * dx + dy * dy <= radius * radius;
    }
}

void BoxCollider::Draw()
{
    if (m_isVisible == false)
        return;

    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();

    Debug::DrawRectangle(pos.x + m_offsetX - m_width * 0.5f, pos.y + m_offsetY - m_height * 0.5f, m_width, m_height, sf::Color::Green);
}

void BoxCollider::SetBox(float _width, float _height)
{
    m_width = _width;
    m_height = _height;
}

float BoxCollider::GetLeft()
{
    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    return pos.x + m_offsetX - m_width * 0.5f;
}

float BoxCollider::GetRight()
{
    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    return pos.x + m_offsetX + m_width * 0.5f;
}

float BoxCollider::GetTop()
{
    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    return pos.y + m_offsetY - m_height * 0.5f;
}

float BoxCollider::GetBottom()
{
    sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
    return pos.y + m_offsetY + m_height * 0.5f;
}

void BoxCollider::Inspect()
{
    sf::Vector2f size = { m_width, m_height };
    if (ImGui::DragFloat2("Box", &size.x))
        SetBox(size.x, size.y);

    Collider::Inspect();
}
