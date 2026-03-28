#include "PlayerHealth.h"
#include "Rigidbody2D.h"
#include "TagComponent.h"
#include "Entity.h"
#include "Collider.h"
#include "imgui.h"
#include "TransformComponent.h"
#include "GameManager.h"
#include "Managers/ShaderManager.h"
#include <iostream>
#include "Sprite.h"
#include <SFML/Graphics.hpp>

#include "SoundEffect.h"
#include "Scene.h"
#include "SceneManager.h"

void PlayerHealth::Update(){

	Rigidbody2D* rb = m_owner->GetComponent<Rigidbody2D>();
	TransformComponent* transform = m_owner->GetComponent<TransformComponent>();

	sf::Vector2f velocity = rb->GetVelocity();
	if (m_health >= 3) {
		SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("healthBar")->GetComponent<Sprite>()->SetSrcRect(0, 0, 1091, 372);
	}
	else if (m_health >= 2) {
		SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("healthBar")->GetComponent<Sprite>()->SetSrcRect(1091, 0, 1091, 372);
	}
	else if (m_health >= 1) {
		SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("healthBar")->GetComponent<Sprite>()->SetSrcRect(2182, 0, 1091, 372);
	}
	else if (m_health >= 0) {
		SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("healthBar")->GetComponent<Sprite>()->SetSrcRect(3273, 0, 1091, 372);
	}

	if (m_DamageTake && !m_invulnerable) {
		m_health--;
		m_invulnerable = true;
		m_invulnerabilityTime = m_invulnerabilityDuration;
		if (knockback == -1) {
			velocity.x = knockbackForce;
			velocity.y = -knockbackForce * 0.5f;
		}
		if (knockback == 1) {
			velocity.x = -knockbackForce;
			velocity.y = -knockbackForce * 0.5f;
		}
		m_owner->GetAllComponents<SoundEffect>()[4]->PlaySoundEffect();
	}

	if (m_invulnerable) {
		ShaderManager::Get()->ApplyTo(m_owner->GetComponent<Sprite>(), "../../../src/Shaders/HologrammeShader.frag");
	}
	else if (!m_invulnerable) {
		ShaderManager::Get()->ApplyTo(m_owner->GetComponent<Sprite>(), "");
	}

	if (m_invulnerable && m_invulnerabilityTime <=0){
		m_invulnerable = false;
	}
	else m_invulnerabilityTime -= m_owner->GetDeltaTime();

	if (m_health <= 0) {
		//std::cout << "Skill Issue" << std::endl;

		SceneManager::GetInstance().ChangeScene("Death");
		SceneManager::GetInstance().FindSceneById("Death")->SetInitalize(true);

		m_health = 3;
		m_Death = true;
	}	
	else if (m_health >= 0) {
		m_Death = false;
	}

	knockback = 0;
	rb->SetVelocity(velocity);
	m_DamageTake = false;
}

void PlayerHealth::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side){
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("DamageZone")) {
		if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Rat") && side == CollisionSide::Bottom) {
			return;
		}
		if (side == CollisionSide::Left) {
			knockback = -1;
		}
		if (side == CollisionSide::Right) {
			knockback = 1;
		}
		m_DamageTake = true;
	}
}

void PlayerHealth::Draw(){

}

void PlayerHealth::Inspect(){
	ImGui::DragFloat("Invulnerability Duration", &m_invulnerabilityDuration, 0.05f);
	ImGui::DragFloat("Knockback Force", &knockbackForce, 0.05f);
	ImGui::DragInt("Health", &m_health, 1);
}

void PlayerHealth::TakeFallDamage(int damage)
{
	if (m_invulnerable) return;
	m_health -= damage;
	m_invulnerable = true;
	m_invulnerabilityTime = m_invulnerabilityDuration;
}

void PlayerHealth::Heal()
{
	m_health++;

	if (m_health >= m_maxhealth)
		m_health = m_maxhealth;
}

json PlayerHealth::Save()
{
	return {
		{"health", m_health},
		{ "invulnerabilityCouldown" , m_invulnerabilityTime}
	};
}

void PlayerHealth::Load(const json& data)
{
	if (data.contains("health"))
		m_health = data["health"];

	if (data.contains("invulnerabilityCouldown"))
		m_invulnerabilityTime = data["invulnerabilityCouldown"];
}

const char* PlayerHealth::GetSaveName() const
{
	return "PlayerHealth";
}
