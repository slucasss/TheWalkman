#pragma once

class Collider;
enum CollisionSide;

class Collidable
{
public:
	virtual void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side) {}
	virtual void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) {}
	virtual void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side) {}
};