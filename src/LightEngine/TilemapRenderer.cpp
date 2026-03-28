#include "TilemapRenderer.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "Managers/AssetsManager.h"

#include "GameManager.h"

TilemapRenderer::TilemapRenderer(const std::vector<int>& _tiles, int _width, int _height, int _tileSize, const std::string& _tilesetPath)
{
    m_tiles = _tiles;
    m_width = _width;
    m_height = _height;
    m_tileSize = _tileSize;

    m_texture = AssetsManager::Get()->Load(_tilesetPath);

    m_tilesetColumns = m_texture->getSize().x / m_tileSize;

    BuildMesh();
}

void TilemapRenderer::Draw()
{
    TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
    
    sf::RenderStates states;
    states.texture = m_texture;

    states.transform = transform->GetTransformable().getTransform();

    GameManager::Get()->GetSceneTexture()->draw(m_vertices, states);
}

void TilemapRenderer::BuildMesh()
{
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(m_width * m_height * 4);

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
        {
            int index = y * m_width + x;
            int id = m_tiles[index];

            if (id == 0)
                continue;

            id--;

            int texX = (id % m_tilesetColumns) * m_tileSize;
            int texY = (id / m_tilesetColumns) * m_tileSize;

            sf::Vertex* quad = &m_vertices[index * 4];

            quad[0].position = { (float)x * m_tileSize, (float)y * m_tileSize };
            quad[1].position = { (float)(x + 1) * m_tileSize, (float)y * m_tileSize };
            quad[2].position = { (float)(x + 1) * m_tileSize, (float)(y + 1) * m_tileSize };
            quad[3].position = { (float)x * m_tileSize, (float)(y + 1) * m_tileSize };

            quad[0].texCoords = { (float)texX, (float)texY };
            quad[1].texCoords = { (float)(texX + m_tileSize), (float)texY };
            quad[2].texCoords = { (float)(texX + m_tileSize), (float)(texY + m_tileSize) };
            quad[3].texCoords = { (float)texX, (float)(texY + m_tileSize) };
        }
    }
}
