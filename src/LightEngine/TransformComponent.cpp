#include "TransformComponent.h"
#include "imgui.h"
#include <cmath>

TransformComponent::TransformComponent() : m_flip(Flip::None)
{
    m_transform.setScale(1.f, 1.f);
}

void TransformComponent::SetPosition(float x, float y)
{
    m_transform.setPosition(x, y);
}

void TransformComponent::SetPosition(sf::Vector2f pos)
{
    SetPosition(pos.x, pos.y);
}

void TransformComponent::Translate(float dx, float dy)
{
    m_transform.move(dx, dy);
}

void TransformComponent::Translate(sf::Vector2f d)
{
    Translate(d.x, d.y);
}

sf::Vector2f TransformComponent::GetPosition() const
{
    return m_transform.getPosition();
}

void TransformComponent::SetRotation(float angle)
{
    m_transform.setRotation(angle);
}

float TransformComponent::GetRotation() const
{
    return m_transform.getRotation();
}

void TransformComponent::SetScale(float sx, float sy)
{
    m_transform.setScale(std::abs(sx), std::abs(sy));
}

void TransformComponent::SetScale(sf::Vector2f scale)
{
    SetScale(scale.x, scale.y);
}

sf::Vector2f TransformComponent::GetScale() const
{
    return m_transform.getScale();
}

void TransformComponent::SetOrigin(float x, float y)
{
    m_transform.setOrigin(x, y);
}

void TransformComponent::SetOrigin(sf::Vector2f origin)
{
    SetOrigin(origin.x, origin.y);
}

sf::Vector2f TransformComponent::GetOrigin() const
{
    return m_transform.getOrigin();
}

void TransformComponent::SetFlip(Flip flip)
{
    m_flip = flip;

    sf::Vector2f scale = m_transform.getScale();
    scale.x = std::abs(scale.x);
    scale.y = std::abs(scale.y);

    switch (flip)
    {
    case Flip::Horizontal:
        scale.x = -scale.x;
        break;

    case Flip::Vertical:
        scale.y = -scale.y;
        break;

    case Flip::Both:
        scale.x = -scale.x;
        scale.y = -scale.y;
        break;

    default:
        break;
    }

    m_transform.setScale(scale);
}

Flip TransformComponent::GetFlip() const
{
    return m_flip;
}

void TransformComponent::Reset()
{
    m_transform.setPosition(0.f, 0.f);
    m_transform.setRotation(0.f);
    m_transform.setScale(1.f, 1.f);
    m_transform.setOrigin(0.f, 0.f);
    m_flip = Flip::None;
}

const sf::Transformable& TransformComponent::GetTransformable() const
{
    return m_transform;
}

void TransformComponent::Inspect()
{
    sf::Vector2f pos = m_transform.getPosition();
    if (ImGui::DragFloat2("Position", &pos.x))
        m_transform.setPosition(pos);

    float rot = m_transform.getRotation();
    if (ImGui::SliderFloat("Rotation", &rot, -180, 180))
        m_transform.setRotation(rot);

    //TODO : Rajouter un CheckBox pour LinkScale
    sf::Vector2f scale = m_transform.getScale();
    if (ImGui::DragFloat2("Scale", &scale.x, 0.005f))
        m_transform.setScale(scale);
}

json TransformComponent::Save()
{
    sf::Vector2f pos = m_transform.getPosition();
    return {
        {"x", pos.x},
        {"y", pos.y}
    };
}

void TransformComponent::Load(const json& data)
{
    m_transform.setPosition({ data["x"], data["y"] });
}

const char* TransformComponent::GetSaveName() const
{
    return "TransformComponent";
}
