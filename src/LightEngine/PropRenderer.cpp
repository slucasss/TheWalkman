#include "PropRenderer.h"
#include "GameManager.h"
#include "TransformComponent.h"

PropRenderer::PropRenderer(sf::Texture* texture)
{
    m_texture = texture;
    m_vertices.setPrimitiveType(sf::Quads);
}

void PropRenderer::AddProp(const sf::Vector2f& pos, const sf::IntRect& rect, const sf::Vector2f& scale, float rotation, Flip flip)
{
    float w = rect.width * scale.x;
    float h = rect.height * scale.y;

    sf::Vector2f p0(0.f, -h);
    sf::Vector2f p1(w, -h);
    sf::Vector2f p2(w, 0.f);
    sf::Vector2f p3(0.f, 0.f);

    float rad = rotation * 3.14159265f / 180.f;
    float cosR = std::cos(rad);
    float sinR = std::sin(rad);
    auto rotate = [&](sf::Vector2f p)
        {
            return sf::Vector2f(p.x * cosR - p.y * sinR, p.x * sinR + p.y * cosR);
        };

    p0 = rotate(p0) + pos;
    p1 = rotate(p1) + pos;
    p2 = rotate(p2) + pos;
    p3 = rotate(p3) + pos;

    float left = (float)rect.left;
    float right = (float)(rect.left + rect.width);
    float top = (float)rect.top;
    float bottom = (float)(rect.top + rect.height);

    if (flip == Flip::Horizontal || flip == Flip::Both) std::swap(left, right);
    if (flip == Flip::Vertical || flip == Flip::Both) std::swap(top, bottom);

    sf::Vertex quad[4];
    quad[0].position = p0; quad[0].texCoords = { left, top };
    quad[1].position = p1; quad[1].texCoords = { right, top };
    quad[2].position = p2; quad[2].texCoords = { right, bottom };
    quad[3].position = p3; quad[3].texCoords = { left, bottom };

    for (int i = 0; i < 4; i++)
        m_vertices.append(quad[i]);
}

void PropRenderer::Draw()
{
    sf::RenderStates states;
    states.texture = m_texture;

    GameManager::Get()->GetSceneTexture()->draw(m_vertices, states);
}