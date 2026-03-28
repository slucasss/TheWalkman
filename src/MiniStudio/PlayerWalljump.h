#pragma once

#include "Component.h"
#include "Updatable.h"
#include "FixedUpdatable.h"
#include "Collidable.h"
#include "Inspectable.h"

class PlayerWalljump :public Component, public Updatable, public FixedUpdatable, public Collidable, public Inspectable
{
	int m_WallSide = 0;
	float m_PushForce = 1.2f;

	bool m_Wall = false;
	bool m_isGrounded = false;
	bool m_groundedThisFrame = false;
	bool m_jumpRight;
	bool m_jumpLeft;
	bool m_wallJumpedThisFrame = false;
	bool m_wallThisFrame = false;

public:
	void FixedUpdate() override;
	void Update() override;

	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side) override;

	void Inspect() override;

	bool IsOnWall() const { return m_Wall; }
	int GetWallSide() const { return m_WallSide; }
	bool WallJumpedThisFrame() const { return m_wallJumpedThisFrame; }
};

