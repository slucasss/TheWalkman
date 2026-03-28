#pragma once
#include "Component.h"
#include "Drawable.h"
#include <SFML/Graphics.hpp>

enum class Flip;

class PropRenderer : public Component, public Drawable
{
public:
    PropRenderer(sf::Texture* texture);

    void AddProp(const sf::Vector2f& pos, const sf::IntRect& rect, const sf::Vector2f& scale, float rotation, Flip flip);

    void Draw() override;

private:
    sf::Texture* m_texture;
    sf::VertexArray m_vertices;
};