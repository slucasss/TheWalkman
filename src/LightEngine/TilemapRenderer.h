#pragma once
#include "Component.h"
#include "Drawable.h"
#include <vector>
#include <string>

class TilemapRenderer : public Component, public Drawable
{
public:
    TilemapRenderer(const std::vector<int>& _tiles, int _width, int _height, int _tileSize, const std::string& _tilesetPath);

    void Draw() override;

private:
    void BuildMesh();

    std::vector<int> m_tiles;
    int m_width;
    int m_height;
    int m_tileSize;

    int m_tilesetColumns;

    sf::Texture* m_texture;
    sf::VertexArray m_vertices;
};
