#pragma once
#include "Component.h"
#include "Updatable.h"
#include "RigidBody2D.h"
#include "Sprite.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "RenderLayerComponent.h"

class RythmeText: public Component, public Updatable{

	float m_lifeTimer = 0.0f;


public:
	RythmeText() {
	};

	void Update() override;

};

