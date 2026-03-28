#pragma once

#include "Component.h"
#include "Updatable.h"
#include "Drawable.h"
#include "Collidable.h"
#include "Inspectable.h"
#include "Savable.h"
#include <SFML/Graphics.hpp>

class PlayerHealth :public Component, public Updatable, public Drawable, public Collidable, public Inspectable, public Savable{
	int m_health = 3;
	int m_maxhealth = 3;
	float m_invulnerabilityTime = 0.f;
	float m_invulnerabilityDuration = 2.f;
	int knockback = 0;
	float knockbackForce = 500.f;

	bool m_Death = false;
	bool m_invulnerable = false;
	bool m_DamageTake = false;

	friend class PlayerMovement;

public:

	void Update() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;

	bool GetDeath() { return m_Death; }

	void Draw() override;
	
	
	void Inspect() override;

public:

	void TakeDamage() { m_DamageTake = true; }
	void TakeFallDamage(int damage);

	void Heal();

	json Save() override;
	void Load(const json& data) override;

	const char* GetSaveName() const override;
};

