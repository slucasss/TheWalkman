#pragma once
#include "Component.h"
#include "Collidable.h"
#include "Updatable.h"

#include <functional>

class TransformComponent;

class DoorLogic : public Component, public Collidable, public Updatable
{
	std::function<void(TransformComponent* playerTransform)> m_onOpen;
	std::function<void()> m_onClick;

	float m_onEnterWait = 0.f;
	float m_waitTimer;
	bool m_clicked = false;


public:
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;
	void SetOnOpen(std::function<void(TransformComponent* playerTransform)> func) { m_onOpen = func; }
	void SetOnClick(std::function<void()> func) { m_onClick = func; }
	
	void Update() override;
	
	void SetWaitOnEnter(float time) { m_onEnterWait = time; }
};

