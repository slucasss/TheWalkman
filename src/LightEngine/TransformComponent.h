#pragma once
#include <SFML/Graphics.hpp>
#include "Component.h"
#include "Inspectable.h"
#include "Savable.h"

enum class Flip
{
    None,
    Horizontal,
    Vertical,
    Both
};

class TransformComponent : public Component, public Inspectable, public Savable
{
public:
    TransformComponent();

    void SetPosition(float x, float y);
    void SetPosition(sf::Vector2f pos);
    void Translate(float dx, float dy);
    void Translate(sf::Vector2f d);
    sf::Vector2f GetPosition() const;

    void SetRotation(float angle);
    float GetRotation() const;

    void SetScale(float sx, float sy);
    void SetScale(sf::Vector2f scale);
    sf::Vector2f GetScale() const;

    void SetOrigin(float x, float y);
    void SetOrigin(sf::Vector2f origin);
    sf::Vector2f GetOrigin() const;

    void SetFlip(Flip flip);
    Flip GetFlip() const;

    void Reset();

    const sf::Transformable& GetTransformable() const;

    void Inspect() override;

    json Save() override;
    void Load(const json& data) override;

    const char* GetSaveName() const override;

private:
    sf::Transformable m_transform;
    Flip m_flip;
};