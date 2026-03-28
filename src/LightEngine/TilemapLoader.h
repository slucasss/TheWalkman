#pragma once
#include <string>
#include <functional>
#include "tinyxml2.h"

#include <SFML/System/Vector2.hpp>

class Scene;

class TilemapLoader
{
public:

    using ObjectLayerHandler = std::function<void(tinyxml2::XMLElement* object, Scene* scene, sf::Vector2f scale)>;

    void AddObjectLayerHandler(const std::string& layerName, ObjectLayerHandler handler);

    void Load(const std::string& path, Scene* scene, const std::string& tilesetPath, sf::Vector2f scale = {1.f, 1.f});

private:
    std::unordered_map<std::string, ObjectLayerHandler> m_objectLayerHandlers;
};
