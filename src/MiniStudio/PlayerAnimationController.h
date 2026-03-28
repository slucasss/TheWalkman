#pragma once
#include "Component.h"
#include "Updatable.h"
#include "Inspectable.h"


enum class PlayerAnimationState
{
    Idle,
    Walk,
    Run,
    PreJump, 
    Jump,
    Fall,
    Land,
    WallGrab,
    WallJump
};

class PlayerAnimationController : public Component, public Updatable, public Inspectable
{
public:
	void Update() override;
	void Inspect() override;

private:
	void UpdateState();
	void ApplyAnimation();
	void ApplyFlip();

    PlayerAnimationState m_currentState = PlayerAnimationState::Idle;
    PlayerAnimationState m_previousState = PlayerAnimationState::Idle;

    float m_landDuration = 0.3f;
    float m_landTimer = 0.f;

    float m_fallThreshold = 50.f;
};

