#include "ImageLayerRenderer.h"
#include "TransformComponent.h"
#include "Managers/AssetsManager.h"
#include "GameManager.h"
#include "Entity.h"

ImageLayerRenderer::ImageLayerRenderer(const std::string& path)
{
    m_sprite.setTexture(*AssetsManager::Get()->Load(path));
}

void ImageLayerRenderer::SetParallax(float x, float y)
{
    m_parallaxX = x;
    m_parallaxY = y;
}

void ImageLayerRenderer::SetFixed(bool value)
{
    m_fixed = value;
}

void ImageLayerRenderer::Draw()
{
    TransformComponent* transform = m_owner->GetComponent<TransformComponent>();

    sf::Vector2f pos = transform->GetPosition();

    if (m_fixed == false)
    {
        sf::View view = GameManager::Get()->GetSceneTexture()->getView();
        sf::Vector2f cam = view.getCenter();

        pos.x -= cam.x * (m_parallaxX - 1.f);
        pos.y -= cam.y * (m_parallaxY - 1.f);
    }

    m_sprite.setPosition(pos);
    m_sprite.setScale(transform->GetScale());

    GameManager::Get()->GetSceneTexture()->draw(m_sprite);
}