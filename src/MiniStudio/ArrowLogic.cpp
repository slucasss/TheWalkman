#include "ArrowLogic.h"
#include "Entity.h"
#include "Collider.h"
#include "TagComponent.h"
#include <iostream>
#include <TransformComponent.h>
#include <Debug.h>

#include "SceneManager.h"
#include "HypeBar.h"

#include "RenderLayerComponent.h"
#include "RythmeText.h"


void ArrowLogic::Update()
{
	float dt = m_owner->GetDeltaTime();

	ArrowMove(-150 * dt, 0);

	//Translate vers la gauche (-x)
}

void ArrowLogic::ArrowMove(float x, float y)
{
	m_owner->GetComponent<TransformComponent>()->Translate(x, y);
}

void ArrowLogic::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side)
{
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Stopper"))
	{
		SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("hypeBar")->GetComponent<HypeBar>()->AddHype(-5);



		Entity* entity = SceneManager::GetInstance().GetCurrentScene()->CreateEntityNow();
		entity->AddComponent<RythmeText>();

		Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/miss.png");
		entity->GetComponent<TransformComponent>()->SetScale({ 0.2, 0.2 });

		Rigidbody2D* rigid = entity->AddComponent<Rigidbody2D>();

		int xImpulse = (rand() % (300 - (-300) + 1)) + (-300);
		int yImpulse = -((rand() % (500 - (700) + 1)) + (700));

		rigid->AddImpulse({ (float)xImpulse, (float)yImpulse });

		entity->GetComponent<TransformComponent>()->SetPosition(m_owner->GetComponent<TransformComponent>()->GetPosition());
		entity->GetComponent<RenderLayerComponent>()->SetLayer(200);

		m_owner->Destroy();
	}

}
