#include "DoorLogic.h"
#include "Collider.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Managers/InputManager.h"
#include "TagComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Camera.h"

#include "GameManager.h"

void DoorLogic::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side)
{
	Entity* otherEntity = _other->GetOwner();

	if (otherEntity->GetComponent<TagComponent>()->Is("Player") == false)
		return;

	if (InputManager::Get()->IsDown(sf::Keyboard::E) || InputManager::Get()->IsDown(2))
	{
		if (m_onClick) {
			m_onClick();
		}
		m_clicked = true;
		m_waitTimer = m_onEnterWait;
	}
	if (m_waitTimer <= 0 && m_clicked) {
		SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Camera")->GetComponent<Camera>()->FadeIn(2);

		TransformComponent* playerTransform = otherEntity->GetComponent<TransformComponent>();
		m_clicked = false;
		if (m_onOpen)
			m_onOpen(playerTransform);
	}
}

void DoorLogic::Update(){
	m_waitTimer -= GameManager::Get()->GetDeltaTime();
}
