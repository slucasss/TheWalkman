#include "TilemapLoader.h"
#include "Scene.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "TilemapRenderer.h"
#include "BoxCollider.h"
#include "TagComponent.h"
#include "ImageLayerRenderer.h"
#include "RenderLayerComponent.h"
#include "TiledUtils.h"

#include <vector>
#include <sstream>
#include <filesystem>

using namespace tinyxml2;

void TilemapLoader::AddObjectLayerHandler(const std::string& layerName, ObjectLayerHandler handler)
{
    m_objectLayerHandlers[layerName] = handler;
}

void TilemapLoader::Load(const std::string& path, Scene* scene, const std::string& tilesetPath, sf::Vector2f scale)
{
    XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != XML_SUCCESS)
        return;

    XMLElement* map = doc.FirstChildElement("map");

    int width = map->IntAttribute("width");
    int height = map->IntAttribute("height");
    int tileSize = map->IntAttribute("tilewidth");

    for (XMLElement* child = map->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
    {
        std::string type = child->Name();

        if (type == "layer") // Tile layer
        {
            XMLElement* data = child->FirstChildElement("data");
            if (!data) continue;

            std::vector<int> tiles;
            std::stringstream ss(data->GetText());
            std::string value;
            while (std::getline(ss, value, ','))
                tiles.push_back(std::stoi(value));

            Entity* mapEntity = scene->CreateEntityNow();
            mapEntity->GetComponent<TagComponent>()->AddTag("Tilemap");

            TransformComponent* transform = mapEntity->GetComponent<TransformComponent>();
            transform->SetPosition({ 0, 0 });
            transform->SetScale(scale);

            mapEntity->AddComponent<TilemapRenderer>(tiles, width, height, tileSize, tilesetPath);
        }
        else if (type == "imagelayer") // Image layer
        {
            XMLElement* image = child->FirstChildElement("image");
            if (!image) continue;

            const char* source = image->Attribute("source");

            float offsetX = child->FloatAttribute("offsetx", 0);
            float offsetY = child->FloatAttribute("offsety", 0);
            float parallaxX = child->FloatAttribute("parallaxx", 1.f);
            float parallaxY = child->FloatAttribute("parallaxy", 1.f);
            bool fixed = child->BoolAttribute("fixed", false);

            int layer = TiledUtils::GetProperty<int>(child, "RenderLayer", 0);

            Entity* entity = scene->CreateEntityNow();
            entity->GetComponent<TagComponent>()->AddTag("ImageLayer");

            TransformComponent* transform = entity->GetComponent<TransformComponent>();
            transform->SetPosition({ offsetX * scale.x, offsetY * scale.y });
            transform->SetScale(scale);

            entity->GetComponent<RenderLayerComponent>()->SetLayer(layer);

            std::string fullPath = TiledUtils::ResolveTiledPath(path, source);

            ImageLayerRenderer* renderer = entity->AddComponent<ImageLayerRenderer>(fullPath);
            renderer->SetParallax(parallaxX, parallaxY);
            renderer->SetFixed(fixed);
        }
        else if (type == "objectgroup") // Object layer
        {
            const char* layerName = child->Attribute("name");
            if (!layerName) continue;

            auto it = m_objectLayerHandlers.find(layerName);
            if (it == m_objectLayerHandlers.end()) continue;

            for (XMLElement* obj = child->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement("object"))
            {
                it->second(obj, scene, scale);
            }
        }
    }
}
