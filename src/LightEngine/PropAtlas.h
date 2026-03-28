#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>

struct AtlasPage
{
    sf::Texture texture;
    sf::Image image;

    int x = 0;
    int y = 0;
    int rowHeight = 0;
};

struct AtlasRect
{
    int atlasIndex;
    sf::IntRect rect;
};

class PropAtlas
{
public:

    void AddImage(const std::string& path);
    void Build();

    sf::Texture* GetTexture(int atlasIndex);
    AtlasRect GetRect(const std::string& path);
    float GetBuildProgress() const;

private:

    std::vector<std::string> m_images;
    std::unordered_map<std::string, AtlasRect> m_rects;
    std::vector<AtlasPage> m_pages;

    size_t m_buildTotal = 0;
    size_t m_buildCurrent = 0;
};