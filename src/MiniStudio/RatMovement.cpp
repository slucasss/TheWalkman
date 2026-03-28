#include "RatMovement.h"
#include "Rigidbody2D.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Collider.h"
#include "imgui.h"
#include "Animation.h"
#include "SoundEffect.h"
#include <iostream>

void RatMovement::FixedUpdate() {
    Rigidbody2D* rb = m_owner->GetComponent<Rigidbody2D>();
    sf::Vector2f velocity = rb->GetVelocity();

    if (m_Death) {
		m_owner->GetAllComponents<SoundEffect>()[1]->PlaySoundEffect();
		m_owner->AddComponent<Animation>("../../../Assets/rat_walk.json")->SetAnimation("dead");
		if (m_cooldown <= 0) {
			m_owner->Destroy();
		}
		m_cooldown -= m_owner->GetDeltaTime();
        return;
    }

    velocity.x = m_direction * m_Speed * 500.f;

	m_owner->GetAllComponents<SoundEffect>()[0]->PlaySoundEffect();

	rb->SetVelocity(velocity);
	m_Ground = false;
}

void RatMovement::Update() {
    m_turnCooldown -= m_owner->GetDeltaTime();

    if (m_wallThisFrame && m_turnCooldown <= 0.f) {
        m_direction *= -1.f;
        m_turnCooldown = m_turnCooldownTime;

        if (m_direction == 1)
        {
            m_owner->GetComponent<TransformComponent>()->SetFlip(Flip::Horizontal);
        }
        else
        {
            m_owner->GetComponent<TransformComponent>()->SetFlip(Flip::None);
        }
    }

    m_wallThisFrame = false;
}

void RatMovement::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) {
    TagComponent* tag = _other->GetOwner()->GetComponent<TagComponent>();

    if ((tag->Is("Ground") || tag->Is("Player")) && (side == CollisionSide::Left || side == CollisionSide::Right))
        m_wallThisFrame = true;

    if (tag->Is("Player") && side == CollisionSide::Top)
	{
		m_Death = true;
        _self->SetActive(false);
        m_owner->GetComponent<Rigidbody2D>()->SetKinematic(true);
		m_Speed = 0;
	}
}


void RatMovement::Inspect(){
	ImGui::DragFloat("Speed", &m_Speed, 0.05f);
	ImGui::DragFloat("Cooldown", &m_cooldown, 0.05f);
}
