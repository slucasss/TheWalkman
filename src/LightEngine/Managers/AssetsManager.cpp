#include <iostream>
#include "AssetsManager.h"

AssetsManager* AssetsManager::Instance = nullptr;

AssetsManager* AssetsManager::Get(){
	if (Instance == nullptr) {
		Instance = new AssetsManager;
	}
	return Instance;
}

sf::Texture* AssetsManager::Load(const std::string& path){
	if (m_textures.count(path))
		return m_textures[path];

	sf::Texture* texture = new sf::Texture();

	if (!texture->loadFromFile(path))
		std::cout << "Error loading texture: " << path << std::endl;

	m_textures[path] = texture;

	return texture;
}

sf::Image* AssetsManager::LoadImage(const std::string& path)
{
	if (m_images.count(path))
		return m_images[path];

	sf::Image* image = new sf::Image();

	if (!image->loadFromFile(path))
		std::cout << "Error loading image: " << path << std::endl;

	m_images[path] = image;

	return image;
}

void AssetsManager::UnloadImage(const std::string& path) 
{
	auto it = m_images.find(path);

	if (it != m_images.end())
	{
		delete it->second;   
		m_images.erase(it); 
	}
}
