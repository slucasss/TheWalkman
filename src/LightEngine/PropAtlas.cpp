#include "PropAtlas.h"
#include "Managers/AssetsManager.h"
#include <iostream>

void PropAtlas::AddImage(const std::string& path)
{
    if (std::find(m_images.begin(), m_images.end(), path) != m_images.end())
        return;

    //std::cout << path << std::endl;
 
    m_images.push_back(path);
}

void PropAtlas::Build()
{
    const int MAX_SIZE = 4096;

    m_buildTotal = m_images.size();
    m_buildCurrent = 0;

    std::sort(m_images.begin(), m_images.end(), [](const std::string& a, const std::string& b)
        {
            auto ta = AssetsManager::Get()->Load(a);
            auto tb = AssetsManager::Get()->Load(b);
            return ta->getSize().y > tb->getSize().y;
        });

    m_pages.clear();
    m_pages.push_back(AtlasPage());
    m_pages.back().image.create(MAX_SIZE, MAX_SIZE, sf::Color::Transparent);

    for (auto& path : m_images)
    {
        //std::cout << "Progress: " << GetBuildProgress() << "%" << std::endl;
        sf::Image* img = AssetsManager::Get()->LoadImage(path);

        int w = img->getSize().x;
        int h = img->getSize().y;

        AtlasPage* page = &m_pages.back();

        if (page->x + w > MAX_SIZE)
        {
            page->x = 0;
            page->y += page->rowHeight;
            page->rowHeight = 0;
        }

        if (page->y + h > MAX_SIZE)
        {
            m_pages.push_back(AtlasPage());
            page = &m_pages.back();
            page->image.create(MAX_SIZE, MAX_SIZE, sf::Color::Transparent);
        }

        page->image.copy(*img, page->x, page->y);
        AssetsManager::Get()->UnloadImage(path);

        m_rects[path] = { (int)m_pages.size() - 1, sf::IntRect(page->x, page->y, w, h) };

        page->x += w;
        page->rowHeight = std::max(page->rowHeight, h);

        m_buildCurrent++; 
    }

    for (auto& page : m_pages)
    {
        page.texture.loadFromImage(page.image);
    }

    //std::cout << "Images in atlas: " << m_images.size() << std::endl;
}

sf::Texture* PropAtlas::GetTexture(int atlasIndex)
{
    return &m_pages[atlasIndex].texture;
}

AtlasRect PropAtlas::GetRect(const std::string& path)
{
    return m_rects[path];
}

float PropAtlas::GetBuildProgress() const
{
    if (m_buildTotal == 0)
        return 0.f;

    return ((float)m_buildCurrent / (float)m_buildTotal) * 100.f;
}