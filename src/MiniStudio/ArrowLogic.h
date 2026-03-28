#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Collidable.h"
#include "Scene.h"

#include "HypeBar.h"

class ArrowLogic : public Component, public Updatable, public Collidable
{

public :

	void Update() override;
	void ArrowMove(float x, float y);
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side) override;
};

