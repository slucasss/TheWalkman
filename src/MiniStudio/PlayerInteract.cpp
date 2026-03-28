#include "PlayerInteract.h"
#include "TagComponent.h"
#include "Collider.h"
#include "Entity.h"
#include "Sprite.h"
#include "RenderLayerComponent.h"
#include "Rigidbody2D.h"
#include "BoxCollider.h"
#include "PlayerAnimationController.h"
#include "Animation.h"
#include "SceneManager.h"
#include "PlayerHealth.h"
#include "Scene.h"
#include "ItemData.h"
#include "imgui.h"
#include "TextComponent.h"
#include "StartScene.h"
#include "TransformComponent.h"
#include "GameManager.h"
#include<iostream>
#include "RebindButton.h"
#include "Managers/InputManager.h"
#include "Inventory.h"
#include "PlayerMovement.h"
#include "SoundEffect.h"


void PlayerInteract::ShowBubbleAt(Entity* bubble, float x, float y)
{
	Sprite* sprite = bubble->GetComponent<Sprite>();
	sprite->SetOffset({ x, y - 80 });
	sprite->SetVisible(true);
}

void PlayerInteract::HideBubble(Entity* bubble)
{
	Sprite* sprite = bubble->GetComponent<Sprite>();
	sprite->SetVisible(false);
}

void PlayerInteract::ShowSpeechBubbleAt(Entity* speech, float x, float y)
{
	Sprite* sprite = speech->GetComponent<Sprite>();
	TransformComponent* bubbleSpeech = speech->GetComponent<TransformComponent>();

	bubbleSpeech->SetScale({ 20, 5 });
	sprite->SetOffset({ x, y - 20 });
	sprite->SetUI(true);
	sprite->SetVisible(true);
}

void PlayerInteract::HideSpeechBubble(Entity* speech)
{
	Sprite* sprite = speech->GetComponent<Sprite>();
	sprite->SetVisible(false);

	TextComponent* text = speech->GetComponent<TextComponent>();

	if (text)
	{
		text->SetVisible(false);
		text->SetText("", true);
	}
}

std::string PlayerInteract::GetNPCDialogue(Entity* npc)
{
	ItemData* data = npc->GetComponent<ItemData>();

	return data->dialogue;
}

PlayerInteract::PlayerInteract()
{
}

void PlayerInteract::OnCollisionEnter(Collider* _self, Collider* _other, CollisionSide side)
{
	Entity* player = _self->GetOwner();
	PlayerMovement* movement = player->GetComponent<PlayerMovement>();


	Entity* item = _other->GetOwner();
	TransformComponent* posItem = item->GetComponent<TransformComponent>();

	//Entity* bubble = SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Help");
	//Sprite* sprite = bubble->GetComponent<Sprite>();



	

	//sprite->SetOffset({ posItem->GetPosition().x, posItem->GetPosition().y - 50 });


	
}

void PlayerInteract::OnCollisionStay(Collider* _self, Collider* _other, CollisionSide side)
{
	Entity* player = _self->GetOwner();
	PlayerMovement* movement = player->GetComponent<PlayerMovement>();
	Rigidbody2D* playerBody = player->GetComponent<Rigidbody2D>();


	Entity* item = _other->GetOwner();
	TransformComponent* posItem = item->GetComponent<TransformComponent>();

	Entity* speech = SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Text");
	TransformComponent* posSpeech = speech->GetComponent<TransformComponent>();
	Sprite* speechBubble = speech->GetComponent<Sprite>();
	speechBubble->SetUI(true);

	Entity* bubble = SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Help");
	Sprite* sprite = bubble->GetComponent<Sprite>();
	bool isCollectible = _other->GetOwner()->GetComponent<TagComponent>()->Is("Collectible");
	bool isInteractable = _other->GetOwner()->GetComponent<TagComponent>()->Is("Interactable");
	bool isConsommable = _other->GetOwner()->GetComponent<TagComponent>()->Is("Consommable");

	if (isCollectible || isConsommable)
	{
		m_colWithCollectable = true;
		ShowBubbleAt(bubble, posItem->GetPosition().x, posItem->GetPosition().y + 5);

		if (m_interact)
			item->Destroy();
	}
	else if (isInteractable)
		ShowBubbleAt(bubble, posItem->GetPosition().x, posItem->GetPosition().y - 45);

	else
		HideBubble(bubble);

	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Collectible"))
	{
		m_colWithCollectable = true;

		if (m_interact && movement->IsGrounded())
		{
			std::string texturePath = item->GetComponent<ItemData>()->texturePath;
			sf::Vector2f itemScale = posItem->GetScale();

			m_owner->GetAllComponents<SoundEffect>()[8]->PlaySoundEffect();
			Entity* player = SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Player");
			Inventory* inventory = player->GetComponent<Inventory>();

			inventory->AddItem(texturePath, itemScale);

			item->Destroy();
			HideBubble(bubble);

			m_interact = false;
		}

	}

	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Interactable"))
	{
		TextComponent* text = speech->GetComponent<TextComponent>();

		if (m_interact && !m_isDialogueOpen && movement->IsGrounded())
		{
			m_isDialogueOpen = true;
			m_currentDialogue = GetNPCDialogue(item);


			if (text)
			{
				item->GetComponent<RenderLayerComponent>()->SetLayer(100);
				text->SetDrawColor(255, 255, 255, 255);

				posSpeech->SetPosition({ 300.f, 620.f });
				posSpeech->SetScale({ 20.f, 6.f });
				text->SetOffset({ -270 , -80 });
				speech->GetComponent<Sprite>()->SetVisible(true);
				text->SetText(m_currentDialogue, false, 30.f);
				text->SetIsUI(true);
				text->SetVisible(true);

			}

			playerBody->SetKinematic(true);
			HideBubble(bubble);
			m_interact = false;
		}
		else if (m_interact && m_isDialogueOpen && movement->IsGrounded())
		{
			m_isDialogueOpen = false;
			m_currentDialogue = "";

			text->SetText(m_currentDialogue);
			HideSpeechBubble(speech);
			ShowBubbleAt(bubble, posItem->GetPosition().x, posItem->GetPosition().y - 45);

			m_interact = false;
			playerBody->SetVelocity({ 0, 0 });
			playerBody->SetKinematic(false);
		}
	}

	if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Consommable"))
	{
		if (m_interact && movement->IsGrounded())
		{
			playerBody->SetKinematic(true);
			PlayerHealth* health = player->GetComponent<PlayerHealth>();

			player->AddComponent<Animation>("../../../Assets/playerAnim.json")->SetAnimation("drink");

			if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Damage"))
			{
				health->TakeDamage();
			}

			if (_other->GetOwner()->GetComponent<TagComponent>()->Is("Heal"))
			{
				health->Heal();
			}

			item->Destroy();
			HideBubble(bubble);
			m_interact = false;
		}
	}
}

void PlayerInteract::OnCollisionExit(Collider* _self, Collider* _other, CollisionSide side)
{
	Entity* player = _self->GetOwner();
	Rigidbody2D* playerBody = player->GetComponent<Rigidbody2D>();

	Entity* bubble = SceneManager::GetInstance().GetCurrentScene()->FindEntityWithTag("Help");

	if (m_colWithCollectable)
	{
		HideBubble(bubble);
	}
	else
		m_colWithCollectable = false;

	m_isDialogueOpen = false;
	m_currentDialogue = "";
}

void PlayerInteract::Inspect()
{
	ImGui::Checkbox("Interact", &m_interact);
}

void PlayerInteract::Update()
{

	m_interact = InputManager::Get()->IsDown(KeyBinds::Get()->GetKeyboardBind("Interact")) || InputManager::Get()->IsDown(KeyBinds::Get()->GetControllerBind("Interact"));

}