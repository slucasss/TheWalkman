#include "PlayerMovement.h"
#include "Managers/InputManager.h"
#include "Rigidbody2D.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Collider.h"
#include "imgui.h"
#include "Animation.h"
#include <iostream>
#include "Sound.h"
#include "KeyBinds.h"
#include "PlayerHealth.h"

#include "Camera.h"


void PlayerMovement::FixedUpdate()
{

	if (IsGrounded() == true) {
		sf::Vector2f velocity = m_owner->GetComponent<Rigidbody2D>()->GetVelocity();

		if (velocity.y >= 500.f) {
			PlayerHealth* playerHealth = m_owner->GetComponent<PlayerHealth>();
			playerHealth->m_health -= int(velocity.y / 800.f);
		}							
	}
	
	Rigidbody2D* rb = m_owner->GetComponent<Rigidbody2D>();
	TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
	Animation* anim = m_owner->GetComponent<Animation>();

	sf::Vector2f velocity = rb->GetVelocity();

	float velY = rb->GetVelocity().y;
	if (!m_isGrounded && velY > 0.f) // on tombe
		m_fallVelocityPeak = std::max(m_fallVelocityPeak, velY);

	// Atterrissage d�tect� : on vient de toucher le sol
	if (m_isGrounded && !m_wasGrounded)
	{
		if (m_fallVelocityPeak > m_fallDamageThreshold)
		{
			PlayerHealth* health = m_owner->GetComponent<PlayerHealth>();
			if (health)
			{
				int damage = (int)((m_fallVelocityPeak - m_fallDamageThreshold) * m_fallDamageMultiplier);
				damage = std::max(1, damage); // minimum 1 d�g�t
				health->TakeFallDamage(damage);
			}
		}
		m_fallVelocityPeak = 0.f;
	}
	m_wasGrounded = m_isGrounded;

	if (m_moveRight && !m_moveLeft) //Movement droite
	{
		if (!m_isGrounded) {
			velocity.x += m_speed * m_resistance;
		}
		else if (m_sprint) {
			velocity.x = m_speed * 700.f * m_HoldTime;
			if (m_HoldTime > m_HoldTimeMax)
				m_HoldTime = m_HoldTimeMax;
			else m_HoldTime += m_owner->GetDeltaTime();
		}
		else{
			
			velocity.x = m_speed * 500.f * m_HoldTime;
			if (m_HoldTime > m_HoldTimeMax)
				m_HoldTime = m_HoldTimeMax;
			else m_HoldTime += m_owner->GetDeltaTime();
		}
	}
	else if (m_moveLeft && !m_moveRight) //Movement Gauche
	{
		if (!m_isGrounded) {
			velocity.x -= m_speed * m_resistance;
		}
		else if (m_sprint) {
			velocity.x = -m_speed * 700.f * m_HoldTime;
			if (m_HoldTime > m_HoldTimeMax)
				m_HoldTime = m_HoldTimeMax;
			else m_HoldTime += m_owner->GetDeltaTime();
		}
		else {
			velocity.x = -m_speed * 500.f * m_HoldTime;
			if (m_HoldTime > m_HoldTimeMax)
				m_HoldTime = m_HoldTimeMax;
			else m_HoldTime += m_owner->GetDeltaTime();
		}
	}
	else
	{
		m_HoldTime = 0.f;

		if (m_isGrounded)
		{
			velocity.x = 0.f;
		}

		if (velocity.x > 0.f)
		{
			velocity.x -= 10.f;
		}
		else if (velocity.x < 0.f)
		{
			velocity.x += 10.f;
		}
	}
	
	if (m_jump) {
		velocity.y = -m_jumpForce * 500.f;
		m_jump = false;
	}

	if (velocity.x > m_speedLimit) {
		velocity.x = m_isGrounded ? m_speedLimit : m_speedLimit * 0.8f;
	}
	else if (velocity.x < -m_speedLimit) {
		velocity.x = m_isGrounded ? -m_speedLimit : -m_speedLimit * 0.8f;
	}

	rb->SetVelocity(velocity);
}

void PlayerMovement::Update(){
	//m_owner->GetComponent<Rigidbody2D>()->SetKinematic(false);
	if (SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Camera")->GetComponent<Camera>()->ShakeIsEnded() == false)
		m_owner->GetComponent<Rigidbody2D>()->SetKinematic(true);

	m_isGrounded = m_groundedThisFrame;
	m_groundedThisFrame = false;
	m_jumpedThisFrame = false;

	m_sprint = InputManager::Get()->IsHeld(KeyBinds::Get()->GetKeyboardBind("Run")) || InputManager::Get()->IsHeld(KeyBinds::Get()->GetControllerBind("Run"));
	m_moveRight = InputManager::Get()->IsHeld(KeyBinds::Get()->GetKeyboardBind("GoRight")) || InputManager::Get()->GetLeftJoystick().x >= 25.f;
	m_moveLeft = InputManager::Get()->IsHeld(KeyBinds::Get()->GetKeyboardBind("GoLeft")) || InputManager::Get()->GetLeftJoystick().x <= -25.f;

	if (m_moveRight) 
		m_moveRightTimer = m_moveBufferTime;
	else 
		m_moveRightTimer -= m_owner->GetDeltaTime();

	if (m_moveLeft) 
		m_moveLeftTimer = m_moveBufferTime;
	else
		m_moveLeftTimer -= m_owner->GetDeltaTime();

	m_moveRight = m_moveRightTimer > 0.f;
	m_moveLeft = m_moveLeftTimer > 0.f;

	if (InputManager::Get()->IsDown(KeyBinds::Get()->GetKeyboardBind("Jump")) && m_isGrounded || InputManager::Get()->IsDown(KeyBinds::Get()->GetControllerBind("Jump")) && m_isGrounded) {
		m_jump = true;
		m_jumpedThisFrame = true;
	}


	TransformComponent* transform = m_owner->GetComponent<TransformComponent>();

	sf::Vector2f velocity = m_owner->GetComponent<Rigidbody2D>()->GetVelocity();
	//std::cout << transform->GetPosition().x << "  " << transform->GetPosition().y << std::endl;
	//std::cout << velocity.x << "  " << velocity.y << std::endl;
}

void PlayerMovement::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side)
{
	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Ground"))
	{
		if (side == CollisionSide::Bottom)
		{
			m_groundedThisFrame = true;
		}

		// Step height
		if (m_isGrounded && (side == CollisionSide::Left || side == CollisionSide::Right))
		{
			TransformComponent* selfTransform = _self->GetOwner()->GetComponent<TransformComponent>();
			TransformComponent* otherTransform = _other->GetOwner()->GetComponent<TransformComponent>();

			float selfBottom = _self->GetBottom();
			float otherBottom = _other->GetBottom();
			float otherTop = _other->GetTop();

			float stepSize = selfBottom - otherTop;

			if (stepSize > 0.f && stepSize <= m_stepHeight)
			{
				selfTransform->SetPosition({selfTransform->GetPosition().x,selfTransform->GetPosition().y - stepSize - 0.1f});
			}
		}
	}
}

void PlayerMovement::Inspect()
{
	ImGui::DragFloat("Speed", &m_speed, 0.05f);
	ImGui::DragFloat("Jump Force", &m_jumpForce, 0.05f);
	ImGui::DragFloat("Max Speed Boost", &m_HoldTimeMax, 0.05f);
	ImGui::DragFloat("Resistance", &m_resistance, 0.05f);
	ImGui::DragFloat("Speed Limit", &m_speedLimit, 0.05f);
	ImGui::Checkbox("Is Grounded", &m_isGrounded);
	ImGui::DragFloat("Step Height", &m_stepHeight, 0.05f);
}

