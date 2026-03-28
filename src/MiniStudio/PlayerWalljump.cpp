#include "PlayerWalljump.h"
#include "Managers/InputManager.h"
#include "Rigidbody2D.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Collider.h"
#include "imgui.h"
#include <iostream>

#include "KeyBinds.h"

void PlayerWalljump::FixedUpdate()
{
	Rigidbody2D* rb = m_owner->GetComponent<Rigidbody2D>();
	TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
	sf::Vector2f velocity = rb->GetVelocity();

	if (m_jumpLeft) {
		velocity.x = m_PushForce * -500.f;
		velocity.y = -500.f;
		m_wallJumpedThisFrame = true;
	}
	if(m_jumpRight){
		velocity.x = m_PushForce * 500.f;
		velocity.y = -500.f;
		m_wallJumpedThisFrame = true;
	}
	if (m_Wall && InputManager::Get()->IsHeld(KeyBinds::Get()->GetKeyboardBind("GoRight")) && velocity.y >= 200|| m_Wall && InputManager::Get()->IsHeld(KeyBinds::Get()->GetKeyboardBind("GoLeft")) && velocity.y >= 200) {
		velocity.y = 200.f;
	}

	rb->SetVelocity(velocity);
}

void PlayerWalljump::Update()
{
	m_isGrounded = m_groundedThisFrame;
	m_groundedThisFrame = false;

	m_Wall = m_wallThisFrame;

	m_wallThisFrame = false;
	if (m_Wall == false) 
		m_WallSide = 0;

	m_jumpLeft = InputManager::Get()->IsDown(sf::Keyboard::Space) && m_Wall && m_WallSide == -1 && !m_isGrounded || InputManager::Get()->IsDown(1) && m_Wall && m_WallSide == -1 && !m_isGrounded;
	m_jumpRight = InputManager::Get()->IsDown(sf::Keyboard::Space) && m_Wall && m_WallSide == 1 && !m_isGrounded || InputManager::Get()->IsDown(1) && m_Wall && m_WallSide == 1 && !m_isGrounded;

	m_wallJumpedThisFrame = m_jumpLeft || m_jumpRight;
}

void PlayerWalljump::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side)
{
	TagComponent* tc = _other->GetOwner()->GetComponent<TagComponent>();
	if (tc->Is("Ground") && tc->Is("!walljump") == false) {
		switch (side)
		{
		case Left:
			m_wallThisFrame = true;
			m_WallSide = 1;
			break;
		case Right:
			m_wallThisFrame = true;
			m_WallSide = -1;
			break;
		case Bottom:
			m_groundedThisFrame = true;
			break;
		}
	}
}

void PlayerWalljump::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side)
{
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Ground"))
		m_Wall = false;
}

void PlayerWalljump::Inspect()
{
	ImGui::DragFloat("Push Force", &m_PushForce, 0.05f);
}
