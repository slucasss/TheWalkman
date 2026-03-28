#pragma once
#include "Component.h"
#include "Updatable.h"
#include "FixedUpdatable.h"
#include "Collidable.h"
#include "Inspectable.h"

class PlayerMovement : public Component, public Updatable, public FixedUpdatable, public Collidable, public Inspectable
{
    bool m_isGrounded = false;
    bool m_groundedThisFrame = false;
    float m_speed = 0.7f;
    float m_jumpForce = 1.3f;
    bool m_jumpedThisFrame = false;
    float m_HoldTime = 0.f;
    float m_HoldTimeMax = 1.5f;
    float m_resistance = 4.f;
    float m_speedLimit = 700.f;
    bool m_moveRight = false;
    bool m_moveLeft = false;
    bool m_jump = false;
    bool m_sprint = false;

    float m_moveRightTimer = 0.f;
    float m_moveLeftTimer = 0.f;
    float m_moveBufferTime = 0.1f;

    float m_fallVelocityPeak = 0.f;      
    float m_fallDamageThreshold = 1200.f; 
    float m_fallDamageMultiplier = 0.01f; 
    bool m_wasGrounded = false;

    float m_stepHeight = 40.f;
    bool m_inBossRoom = false;

public:
    void FixedUpdate() override;
    void Update() override;
    void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;
    void Inspect() override;
    bool IsGrounded() const { return m_isGrounded; }
    bool IsSprinting() const { return m_sprint; }
    bool IsMovingLeft() const { return m_moveLeft; }
    bool IsMovingRight() const { return m_moveRight; }
    bool JumpedThisFrame() const { return m_jumpedThisFrame; }

    bool IsInBossRoom() { return m_inBossRoom; }

    void SetInBossRoom(bool InBossRoom) { 
        m_inBossRoom = InBossRoom;
    }
};