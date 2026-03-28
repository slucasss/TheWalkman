#pragma once
#include "Component.h"
#include "Drawable.h"
#include <SFML/Graphics.hpp>

class ImageLayerRenderer : public Component, public Drawable
{
public:

    ImageLayerRenderer(const std::string& path);

    void SetParallax(float x, float y);
    void SetFixed(bool value);

    void Draw() override;

private:

    sf::Sprite m_sprite;

    float m_parallaxX = 1.f;
    float m_parallaxY = 1.f;

    bool m_fixed = false;
};