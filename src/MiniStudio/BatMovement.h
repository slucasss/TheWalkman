#pragma once
#include "Component.h"
#include "FixedUpdatable.h"
#include "Collidable.h"
#include "Inspectable.h"


class BatMovement : public Component, public FixedUpdatable, public Collidable, public Inspectable
{
	float m_Speed = 0.2f;
	float m_maxDistance = 200.f;
	float m_startPositionX = 0;

	bool m_limitPosition = false;
	bool m_init = false;

	
public:
	void FixedUpdate() override;
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side) override;

	void Inspect() override;
};

