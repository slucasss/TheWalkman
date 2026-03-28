#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Inspectable.h"
#include "Savable.h"
#include <SFML/Graphics.hpp>

class HealthBar : public Component, public Drawable, public Inspectable, public Savable
{
public:
    HealthBar(float width, float height);

    void SetHealth(float value); // 0 -> 1

    void Draw() override;

    void Inspect() override;

    json Save() override;
    void Load(const json& data) override;

    const char* GetSaveName() const override;

private:
    sf::RectangleShape m_bg;
    sf::RectangleShape m_bar;

    float m_maxWidth;
};