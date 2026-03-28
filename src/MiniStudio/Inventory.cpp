#include "Inventory.h"
#include "TransformComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "PlayerInteract.h"
#include "imgui.h"
#include "GameManager.h"
#include "Sprite.h"



void Inventory::AddSlot(Entity* entity)
{
	m_slots.push_back({ entity});
}


bool Inventory::AddItem(const std::string& texturePath, sf::Vector2f itemScale)
{
    for (auto& slot : m_slots)
    {
        Sprite* sprite = slot.entity->GetComponent<Sprite>();

        if (slot.m_empty)
        {
            sprite->SetOffset({ 0, 0 });
            sprite->SetTexture(texturePath);
            slot.entity->GetComponent<TransformComponent>()->SetScale(itemScale);
            slot.m_empty = false;
            m_numItems++;

            if (m_numItems >= 3)
            {
                ResetSlots();
                Sprite* reward = m_slots[0].entity->GetComponent<Sprite>();
                reward->SetOffset({ 75, 25 });
                reward->SetTexture(m_rewardTexturePath);
                reward->SetSrcRect(0, 0, 1026, 690);
                m_slots[0].entity->GetComponent<TransformComponent>()->SetScale({0.25, 0.25});
                m_slots[0].m_empty = false;
                m_numItems = 1;
                m_allItems = true;
            }
            return true;
        }
    }
    return false;
}

void Inventory::ResetSlots()
{
    for (auto& slot : m_slots)
    {
        Sprite* sprite = slot.entity->GetComponent<Sprite>();
        sprite->SetOffset({ 0, 0 });
        sprite->SetTexture("../../../Assets/marco.png");
        slot.entity->GetComponent<TransformComponent>()->SetScale({ 1.f, 1.f });
        slot.m_empty = true;
    }
    m_numItems = 0;
}

void Inventory::Update()
{
}