#pragma once

#include "Component.h"
#include "Updatable.h"
#include "Collidable.h"
#include "Drawable.h"
#include "Inspectable.h"
#include "Savable.h"
#include <SFML/Graphics.hpp>

class MusicChange : public Component, public Updatable, public Collidable, public Inspectable, public Savable
{
	int m_MusicType = 0;
	float m_cooldown = 1.0f;
	float m_timer = 0.f;

	bool m_RockZone = false;
	bool m_SpecialZone = false;

public:
	void Update() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side) override;
	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side) override;

	int GetMusicType() { return m_MusicType; }

	void Inspect() override;

	json Save() override;
	void Load(const json& data) override;
	const char* GetSaveName() const override;
};

