#include "MusicChange.h"
#include "Managers/InputManager.h"
#include "Rigidbody2D.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Collider.h"
#include "imgui.h"
#include "GameManager.h"
#include "PlayerHealth.h"
#include "Scene.h"
#include "Sprite.h"
#include "Music.h"
#include <SFML/Graphics.hpp>

#include "AmbiantShader.h"

void MusicChange::Update(){

	TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();
	Sprite* cassetsprite = currentScene->FindEntityWithTag("MusicChange")->GetComponent<Sprite>();
	Sprite* collapseSprite = currentScene->FindEntityWithTag("ecroulement")->GetComponent<Sprite>();

	if (m_owner->GetComponent<PlayerHealth>()->GetDeath()) {
		m_MusicType = 0;
	}

	if (InputManager::Get()->IsDown(sf::Keyboard::Z) && m_timer <= 0.f && !m_SpecialZone || InputManager::Get()->IsDown(7) && m_timer <= 0.f && !m_SpecialZone) {
		m_timer = m_cooldown;
		m_MusicType++;

		if (m_MusicType > 2) {
			m_MusicType = 1;
		}

		switch (m_MusicType)
		{
		case 1:
			cassetsprite->SetSrcRect(380, 0, 380, 206);
			if (collapseSprite->IsVisible()) collapseSprite->SetSrcRect(2680, 0, 2680, 1000);
			break;
		case 2:
			cassetsprite->SetSrcRect(760, 0, 380, 206);
			if (collapseSprite->IsVisible()) collapseSprite->SetSrcRect(5360, 0, 2680, 1000);
			break;
		default:
			break;
		}

		for (Entity* entity : currentScene->FindAllEntitiesWithTag("Jazz"))
		{
			if (Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(m_MusicType == 1);
			if (Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(m_MusicType == 1);
		}

		for (Entity* entity : currentScene->FindAllEntitiesWithTag("Rock")) 
		{
			if(Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(m_MusicType == 2);
			if(Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(m_MusicType == 2);
		}
		AmbiantShader::Get()->Pulse(0.05);

		if (m_MusicType == 1) {
			SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()[0]->Play();
			SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()[1]->Pause();
		}
		else if (m_MusicType == 2) {
			SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()[1]->Play();
			SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()[0]->Pause();
		}
	}

	if (InputManager::Get()->IsDown(sf::Keyboard::A) && m_timer <= 0.f && !m_SpecialZone || InputManager::Get()->IsDown(6) && m_timer <= 0.f && !m_SpecialZone) {
		m_timer = m_cooldown;
		m_MusicType = 0;

		cassetsprite->SetSrcRect(0, 0, 380, 206);
		if (collapseSprite->IsVisible()) collapseSprite->SetSrcRect(0, 0, 2680, 1000);
		
		for (Entity* entity : currentScene->FindAllEntitiesWithTag("Jazz")) {
			if (Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(false);
			if (Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(false);
		}
		for (Entity* entity : currentScene->FindAllEntitiesWithTag("Rock")) {
			if (Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(false);
			if (Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(false);
		}
		AmbiantShader::Get()->Pulse(0.05);
		if (m_MusicType == 0) {
			SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()[0]->Pause();
			SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()[1]->Pause();
		}
	}

	if (m_RockZone) {
		if (m_MusicType != 2) {
			SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()[1]->Play();
			SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()[0]->Pause();
			AmbiantShader::Get()->Pulse(0.05);
			m_MusicType = 2;
			cassetsprite->SetSrcRect(760, 0, 380, 206);
			if (collapseSprite->IsVisible()) collapseSprite->SetSrcRect(5360, 0, 2680, 1000);
			for (Entity* entity : currentScene->FindAllEntitiesWithTag("Rock"))
			{
				if (Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(m_MusicType == 2);
				if (Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(m_MusicType == 2);
			}
		}

	}

	m_timer -= m_owner->GetDeltaTime();
}

void MusicChange::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side){
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("SpecialZone")) {
		m_SpecialZone = true;
	}
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("RockZone")) {
		m_SpecialZone = true;
		m_RockZone = true;
	}
}

void MusicChange::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side)
{
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("SpecialZone")) {
		m_SpecialZone = false;
	}
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("RockZone")) {
		m_SpecialZone = false;
		m_RockZone = false;
	}
}

void MusicChange::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side)
{
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("RockZone")) {
		m_SpecialZone = true;
		m_RockZone = true;
	}
}

void MusicChange::Inspect(){
	ImGui::DragFloat("Cooldown", &m_cooldown, 0.05f);
}

json MusicChange::Save()
{
	return
	{
		{"Cooldown", m_cooldown},
		{"MusicType", m_MusicType}
	};
}

void MusicChange::Load(const json& data)
{
	if (data.contains("Cooldown"))
		m_cooldown = data.value("Cooldown", m_cooldown);

	if (data.contains("MusicType"))
		m_MusicType = data.value("MusicType", m_MusicType);
}

const char* MusicChange::GetSaveName() const
{
	return "MusicChangeLogic";
}
