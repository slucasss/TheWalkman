#pragma once
#include "Component.h"
#include "Updatable.h"
#include "FixedUpdatable.h"
#include "Collidable.h"
#include "Inspectable.h"

class RatMovement : public Component, public Updatable, public FixedUpdatable, public Collidable, public Inspectable
{
	float m_Speed = 0.5f;
	float m_cooldown = 10.0f;

	bool m_Wall = false;
	bool m_Ground = false;
	bool m_Death = false;

	bool m_wallThisFrame = false;
	float m_direction = -1.f; 

	float m_turnCooldown = 0.f;
	float m_turnCooldownTime = 0.3f;
public:
	void FixedUpdate() override;
	void Update() override;
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;

	void Inspect() override;
};

