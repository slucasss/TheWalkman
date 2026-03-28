#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>


class AssetsManager{
	static AssetsManager* Instance;

	std::unordered_map<std::string, sf::Texture*> m_textures;
	std::unordered_map<std::string, sf::Image*> m_images;

public:
	static AssetsManager* Get();

	sf::Texture* Load(const std::string& path);
	sf::Image* LoadImage(const std::string& path);
	void UnloadImage(const std::string& path);
};

