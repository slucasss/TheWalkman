#include "PlayerAnimationController.h"
#include "PlayerMovement.h"
#include "PlayerWalljump.h"
#include "Animation.h"
#include "TransformComponent.h"
#include "Rigidbody2D.h"
#include "Entity.h"
#include "Sound.h"
#include "imgui.h"
#include <iostream>

#include "SoundEffect.h"


void PlayerAnimationController::Update()
{
    if (m_owner->GetComponent<Rigidbody2D>()->IsKinematic())
        return;
    UpdateState();
    ApplyAnimation();
    ApplyFlip();
}

void PlayerAnimationController::UpdateState()
{
    PlayerMovement* movement = m_owner->GetComponent<PlayerMovement>();
    PlayerWalljump* walljump = m_owner->GetComponent<PlayerWalljump>();
    Rigidbody2D* rb = m_owner->GetComponent<Rigidbody2D>();

    bool grounded = movement->IsGrounded();
    bool onWall = walljump->IsOnWall() && grounded == false;
    bool movingRight = movement->IsMovingRight();
    bool movingLeft = movement->IsMovingLeft();
    bool sprinting = movement->IsSprinting();
    bool jumpedThisFrame = movement->JumpedThisFrame();
    bool wallJumpedThisFrame = walljump->WallJumpedThisFrame();
    float velY = rb->GetVelocity().y;

    m_previousState = m_currentState;

    if (wallJumpedThisFrame)
    {
        m_currentState = PlayerAnimationState::WallJump;
        m_owner->GetAllComponents<SoundEffect>()[3]->StopSoundEffect();
        return;
    }

    if (m_currentState == PlayerAnimationState::WallJump)
    {
        Animation* anim = m_owner->GetComponent<Animation>();
        if (!anim->IsAnimationEnded())
            return;

        float velY = rb->GetVelocity().y;
        m_currentState = velY > m_fallThreshold ? PlayerAnimationState::Fall : PlayerAnimationState::Jump;
        return;
    }

    if (m_currentState == PlayerAnimationState::WallGrab)
    {
        if (onWall == false)
        {
            m_currentState = PlayerAnimationState::Jump;
            return;
        }
    }

    if (onWall && grounded == false)
    {
        m_currentState = PlayerAnimationState::WallGrab;
        return;
    }

    if (jumpedThisFrame)
    {
        m_currentState = PlayerAnimationState::PreJump;
        return;
    }
    if (m_currentState == PlayerAnimationState::PreJump)
    {
        Animation* anim = m_owner->GetComponent<Animation>();
        if (anim->IsAnimationEnded()) 
            m_currentState = PlayerAnimationState::Jump;
        return;
    }

    if (grounded == false)
    {
        if (velY > m_fallThreshold)
        {
            m_currentState = PlayerAnimationState::Fall;
        }
        return;
    }

    if (m_previousState == PlayerAnimationState::Fall || m_previousState == PlayerAnimationState::WallJump)
    {
        m_currentState = PlayerAnimationState::Land;
        m_landTimer = m_landDuration;
        return;
    }

    if (m_currentState == PlayerAnimationState::Land)
    {
        m_landTimer -= m_owner->GetDeltaTime();
        if (m_landTimer > 0.f)
            return;
    }

    if(movingLeft && movingRight)
    {
        movingLeft = false;
        movingRight = false;
    }

    bool moving = movingRight || movingLeft;
    if (moving && sprinting)
        m_currentState = PlayerAnimationState::Run;
    else if (moving)
        m_currentState = PlayerAnimationState::Walk;
    else
        m_currentState = PlayerAnimationState::Idle;
}

void PlayerAnimationController::ApplyAnimation()
{
    Animation* anim = m_owner->GetComponent<Animation>();

    switch (m_currentState)
    {
    case PlayerAnimationState::Idle:
        m_owner->GetAllComponents<SoundEffect>()[3]->StopSoundEffect();
        anim->SetAnimation("idle"); 
        break;
    case PlayerAnimationState::Walk: 
        m_owner->GetAllComponents<SoundEffect>()[3]->StopSoundEffect();
        anim->SetAnimation("walk");
        m_owner->GetAllComponents<SoundEffect>()[0]->PlaySoundEffect();
        break;
    case PlayerAnimationState::Run: 
        m_owner->GetAllComponents<SoundEffect>()[3]->StopSoundEffect();
        anim->SetAnimation("run");
        m_owner->GetAllComponents<SoundEffect>()[1]->PlaySoundEffect();
        break;
    case PlayerAnimationState::PreJump:
        m_owner->GetAllComponents<SoundEffect>()[5]->PlaySoundEffect();
        anim->SetAnimation("pre_jump"); 
        break;
    case PlayerAnimationState::Jump: 
        anim->SetAnimation("jump");
        break;
    case PlayerAnimationState::Fall: 
        anim->SetAnimation("fall"); 
        break;
    case PlayerAnimationState::Land:
        m_owner->GetAllComponents<SoundEffect>()[3]->StopSoundEffect();
        anim->SetAnimation("land"); 
        m_owner->GetAllComponents<SoundEffect>()[6]->PlaySoundEffect();
        break;
    case PlayerAnimationState::WallGrab: 
        anim->SetAnimation("wallgrab"); 
        m_owner->GetAllComponents<SoundEffect>()[3]->PlaySoundEffect();
        break;
    case PlayerAnimationState::WallJump:
        m_owner->GetAllComponents<SoundEffect>()[3]->StopSoundEffect();
        anim->SetAnimation("walljump"); 
        m_owner->GetAllComponents<SoundEffect>()[5]->PlaySoundEffect();
        break;
    }


}

void PlayerAnimationController::ApplyFlip()
{
    TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
    PlayerMovement* movement = m_owner->GetComponent<PlayerMovement>();
    PlayerWalljump* walljump = m_owner->GetComponent<PlayerWalljump>();

    //flip selon le côté du mur
    if (m_currentState == PlayerAnimationState::WallGrab || m_currentState == PlayerAnimationState::WallJump)
    {
        int side = walljump->GetWallSide();
        transform->SetFlip(side == 1 ? Flip::None : Flip::Horizontal);
        return;
    }

    //flip basé sur la direction de déplacement
    if (movement->IsMovingRight() && !movement->IsMovingLeft())
        transform->SetFlip(Flip::None);
    else if (movement->IsMovingLeft() && !movement->IsMovingRight())
        transform->SetFlip(Flip::Horizontal);
}

void PlayerAnimationController::Inspect()
{
    ImGui::DragFloat("Land Duration", &m_landDuration, 0.01f);
    ImGui::DragFloat("Fall Threshold", &m_fallThreshold, 1.f);

    const char* stateNames[] = {"Idle","Walk","Run","PreJump","Jump","Fall","Land","WallGrab","WallJump"};
    ImGui::Text("Current State: %s", stateNames[(int)m_currentState]);
}