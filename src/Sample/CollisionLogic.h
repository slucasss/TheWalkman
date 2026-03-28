#pragma once
#include "Component.h"
#include "Collidable.h"

class CollisionLogic : public Component, public Collidable
{
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side) override;
};