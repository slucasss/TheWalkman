#include "CheckZoneLogic.h"

#include "GameManager.h"
#include "Managers/InputManager.h"
#include "KeyBinds.h"	

#include "Rigidbody2D.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "AllComponent.h"


#include "Entity.h"
#include "PlayerMovement.h"

#include "SceneManager.h"
#include "Scene.h"

#include "Collider.h"
#include "imgui.h"
#include <iostream>
#include "Debug.h"

#include "RythmeText.h"

#include "StartScene.h"
#include "HypeBar.h"


CheckZoneLogic::CheckZoneLogic() {
	m_isRightArrowInZone = false;
	m_isLeftArrowInZone = false;
	m_isUpArrowInZone = false;
	m_isDownArrowInZone = false;

	m_isPlaying = true;

	
}

bool IsInput(const std::string& input)
{
	InputManager* im = InputManager::Get();

	if (input == "Right_Arrow")
	{
		return (im->IsDown(KeyBinds::Get()->GetKeyboardBind("Right_Arrow")) || im->IsDown(1));
	}
	else if (input == "Left_Arrow")
	{
		return (im->IsDown(KeyBinds::Get()->GetKeyboardBind("Left_Arrow")) || im->IsDown(4));
	}
	else if (input == "Up_Arrow")
	{
		return (im->IsDown(KeyBinds::Get()->GetKeyboardBind("Up_Arrow")) || im->IsDown(3));
	}
	else if (input == "Down_Arrow")
	{
		return (im->IsDown(KeyBinds::Get()->GetKeyboardBind("Down_Arrow")) || im->IsDown(2));
	}

	return false;
}

void CheckZoneLogic::Update()
{
	if (m_isPlaying == false)
		return;

	Scene* currentScene = SceneManager::GetInstance().GetCurrentScene();

	Entity* rightArrow = currentScene->FindEntityWithTag("RightArrows");
	Entity* leftArrow = currentScene->FindEntityWithTag("LeftArrows");
	Entity* upArrow = currentScene->FindEntityWithTag("UpArrows");
	Entity* downArrow = currentScene->FindEntityWithTag("DownArrows");

	sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();

	if (rightArrow && IsInput("Right_Arrow")) {
		m_arrowPos = rightArrow->GetComponent<TransformComponent>()->GetPosition();

		if (m_isRightArrowInZone)
		{
			sf::Vector2f vector = m_arrowPos - pos;

			m_dist = sqrt(vector.x * vector.x + 0.f);

		}
		else
		{
			m_dist = -2.f;
		}
		rightArrow->Destroy();
	}

	if (leftArrow && IsInput("Left_Arrow")) {
		m_arrowPos = leftArrow->GetComponent<TransformComponent>()->GetPosition();

		if (m_isLeftArrowInZone)
		{
			sf::Vector2f vector = m_arrowPos - pos;

			m_dist = sqrt(vector.x * vector.x + 0.f);

		}
		else
		{
			m_dist = -2.f;
		}
		leftArrow->Destroy();
	}

	if (upArrow && IsInput("Up_Arrow")) {
		m_arrowPos = upArrow->GetComponent<TransformComponent>()->GetPosition();

		if (m_isUpArrowInZone)
		{
			sf::Vector2f vector = m_arrowPos - pos;

			m_dist = sqrt(vector.x * vector.x + 0.f);

		}
		else
		{
			m_dist = -2.f;
		}
		upArrow->Destroy();
	}

	if (downArrow && IsInput("Down_Arrow")) {
		m_arrowPos = downArrow->GetComponent<TransformComponent>()->GetPosition();

		if (m_isDownArrowInZone)
		{
			sf::Vector2f vector = m_arrowPos - pos;

			m_dist = sqrt(vector.x * vector.x + 0.f);

		}
		else
		{
			m_dist = -2.f;
		}
		downArrow->Destroy();
	}

	HypeBar* hype = SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("hypeBar")->GetComponent<HypeBar>();

	if (m_dist > -1.f && m_dist < 50.f) {

		if (m_dist <= 10)
		{
			Entity* entity = SceneManager::GetInstance().GetCurrentScene()->CreateEntityNow();
			entity->AddComponent<RythmeText>();
			Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/perfect.png");
			entity->GetComponent<TransformComponent>()->SetScale({ 0.2, 0.2 });
			entity->GetComponent<RenderLayerComponent>()->SetLayer(120);
		
			Rigidbody2D* rigid = entity->AddComponent<Rigidbody2D>();

			int xImpulse = (rand() % (300 - (-300) + 1)) + (-300);
			int yImpulse = -((rand() % (500 - (700) + 1)) + (700));

			rigid->AddImpulse({ (float)xImpulse, (float)yImpulse });

			entity->GetComponent<TransformComponent>()->SetPosition(m_arrowPos);
			entity->GetComponent<RenderLayerComponent>()->SetLayer(200);
			hype->AddHype(5.f);
		}

		else if (m_dist <= 25)
		{
			Entity* entity = SceneManager::GetInstance().GetCurrentScene()->CreateEntityNow();
			entity->AddComponent<RythmeText>();
			Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/great.png");
			entity->GetComponent<TransformComponent>()->SetScale({ 0.2, 0.2 });
			entity->GetComponent<RenderLayerComponent>()->SetLayer(120);

			Rigidbody2D* rigid = entity->AddComponent<Rigidbody2D>();

			int xImpulse = (rand() % (300 - (-300) + 1)) + (-300);
			int yImpulse = -((rand() % (500 - (700) + 1)) + (700));

			rigid->AddImpulse({ (float)xImpulse, (float)yImpulse });

			entity->GetComponent<TransformComponent>()->SetPosition(m_arrowPos);
			entity->GetComponent<RenderLayerComponent>()->SetLayer(200);
			hype->AddHype(3.f);
		}

		else
		{
			Entity* entity = SceneManager::GetInstance().GetCurrentScene()->CreateEntityNow();
			entity->AddComponent<RythmeText>();
			Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/bad.png");
			entity->GetComponent<TransformComponent>()->SetScale({ 0.2, 0.2 });
			entity->GetComponent<RenderLayerComponent>()->SetLayer(120);

			Rigidbody2D* rigid = entity->AddComponent<Rigidbody2D>();

			int xImpulse = (rand() % (300 - (-300) + 1)) + (-300);
			int yImpulse = -((rand() % (500 - (700) + 1)) + (700));

			rigid->AddImpulse({ (float)xImpulse, (float)yImpulse });

			entity->GetComponent<TransformComponent>()->SetPosition(m_arrowPos);
			entity->GetComponent<RenderLayerComponent>()->SetLayer(200);
			hype->AddHype(1.f);
		}
	}

	else if (m_dist == -2.f) {
		Entity* entity = SceneManager::GetInstance().GetCurrentScene()->CreateEntityNow();
		entity->AddComponent<RythmeText>();
		Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/miss.png");
		entity->GetComponent<TransformComponent>()->SetScale({ 0.2, 0.2 });
		entity->GetComponent<RenderLayerComponent>()->SetLayer(120);

		Rigidbody2D* rigid = entity->AddComponent<Rigidbody2D>();

		int xImpulse = (rand() % (300 - (-300) + 1)) + (-300);
		int yImpulse = -((rand() % (500 - (700) + 1)) + (700));

		rigid->AddImpulse({ (float)xImpulse, (float)yImpulse });

		entity->GetComponent<TransformComponent>()->SetPosition(m_arrowPos);
		entity->GetComponent<RenderLayerComponent>()->SetLayer(200);
		hype->AddHype(-5.f);
	}
	m_dist = -1.f;
}

void CheckZoneLogic::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side)
{
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("RightArrows"))
		m_isRightArrowInZone = true;

	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("LeftArrows"))
		m_isLeftArrowInZone = true;


	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("UpArrows"))
		m_isUpArrowInZone = true;

	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("DownArrows"))
		m_isDownArrowInZone = true;
}

void CheckZoneLogic::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side)
{
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("RightArrows"))
		m_isRightArrowInZone = false;

	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("LeftArrows"))
		m_isLeftArrowInZone = false;

	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("UpArrows"))
		m_isUpArrowInZone = false;

	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("DownArrows"))
		m_isDownArrowInZone = false;
}
