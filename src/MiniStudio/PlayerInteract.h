#pragma once

#include "Collidable.h"
#include "Component.h"
#include "Entity.h"
#include "Inspectable.h"
#include "StartScene.h"
#include "Updatable.h"
#include "PlayerMovement.h"

#include <vector>

class Rigidbody2D;

class PlayerInteract : public Collidable, public Component, public Inspectable, public Updatable
{
private:

	bool m_interact;
	bool m_colWithCollectable;

	bool m_isDialogueOpen = false;
	std::string m_currentDialogue = "";

public:
	PlayerInteract();

	void OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side) override;
	void OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side) override;
	void OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side) override;

	void Inspect() override;
	void Update() override;

	void ShowBubbleAt(Entity* bubble, float x, float y);
	void HideBubble(Entity* bubble);
	void ShowSpeechBubbleAt(Entity* speech, float x, float y);
	void HideSpeechBubble(Entity* speech);
	std::string GetNPCDialogue(Entity* npc);

	friend class Inventory;
};