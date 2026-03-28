#include "BatMovement.h"
#include "Rigidbody2D.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Collider.h"
#include "imgui.h"
#include "Animation.h"
#include <iostream>
#include "SoundEffect.h"

void BatMovement::FixedUpdate(){

	TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
	if (!m_init) {
		m_startPositionX = transform->GetPosition().x;
		m_init = true;
	}

	sf::Vector2f position = transform->GetPosition();

	if (m_startPositionX - m_maxDistance >= transform->GetPosition().x) {
		m_limitPosition = true;
	}
	else if (m_startPositionX + m_maxDistance <= transform->GetPosition().x) {
		m_limitPosition = false;
	}

	if(m_limitPosition){
		position.x += m_Speed * 5.f;
		transform->SetFlip(Flip::Horizontal);
	}
	else if (!m_limitPosition) {
		position.x -= m_Speed * 5.f;
		transform->SetFlip(Flip::None);
	}
	m_owner->GetComponent<SoundEffect>()->PlaySoundEffect();
	transform->SetPosition(position);
}

void BatMovement::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side){
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Ground") && side == CollisionSide::Left) {
		m_limitPosition = true;
	}
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Ground") && side == CollisionSide::Left) {
		m_limitPosition = true;
	}
}


void BatMovement::Inspect(){
	ImGui::DragFloat("Speed", &m_Speed, 0.05f);
	ImGui::DragFloat("Max Distance", &m_maxDistance, 0.05f);
}
