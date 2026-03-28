#pragma once

#include "Component.h"
#include "Updatable.h"
#include "Entity.h"
#include "Camera.h"
#include "Inspectable.h"
#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Drawable.h"


class Inventory : public Component, public Updatable
{
public:
    struct Slot {
        Entity* entity;
        sf::Vector2f screenOffset;
        bool m_empty = true;
    };

    void AddSlot(Entity* entity);
    bool AddItem(const std::string& texturePath, sf::Vector2f itemScale);
    void Update() override;

    void ResetSlots();
    void SetRewardTexture(const std::string& path) { m_rewardTexturePath = path; }

    bool HasAllItems() { return m_allItems; }

private:
    std::vector<Slot> m_slots;
    int m_numItems = 0;
    std::string m_rewardTexturePath;
    bool m_allItems = false;
};