#include "Scene.h"
#include "PauseScene.h"

#include "Entity.h"
#include "AllComponent.h"
#include "GameManager.h"

#include "StatsWindow.h"
#include "Managers/InputManager.h"
#include "Managers/AssetsManager.h"

#include "Debug.h"

#include "TilemapLoader.h"
#include "TiledUtils.h"

#include <iostream>
#include "PlayerMovement.h"

#include "RebindButton.h"
#include "SceneManager.h"

#include "TransformComponent.h"


namespace {
	void Resume() {
		SceneManager::GetInstance().ChangeScene("Start");
		SceneManager::GetInstance().FindSceneById("Start")->SetInitalize(false);
	}

	void Settings() {
		SceneManager::GetInstance().ChangeScene("Settings");
		SceneManager::GetInstance().FindSceneById("Settings")->SetInitalize(true);
	}

	void Menu() {
		SceneManager::GetInstance().ChangeScene("MainMenu");
		SceneManager::GetInstance().FindSceneById("MainMenu")->SetInitalize(true);
		SceneManager::GetInstance().FindSceneById("Start")->DestroyAllEntities();
	}

	void SaveButton() {
		SceneManager::GetInstance().FindSceneById("Start")->Save("../../../Assets/saveTest.json");
	}

}


void PauseScene::Turn(Entity* target) {
	int i = 0;
	for (auto& e : m_buttons) {
		if (e.entity == target) {
			id = i;

			float currentBouttonAngle = m_currentAngle + m_buttons[id].angleOffset;
			float diff = 0.f - currentBouttonAngle;

			while (diff > M_PI) diff -= 2.f * M_PI;
			while (diff < -M_PI) diff += 2.f * M_PI;


			m_targetAngle = m_currentAngle + diff;

			m_isTurning = true;
			return;
		}
		i++;
	}
}

PauseScene::PauseScene(){
	
}

void PauseScene::OnLoading() {

}

void PauseScene::OnInitialize(){
	GameManager::Get()->mClearColor = sf::Color{ 246, 236, 215, 0 };

	{
		Entity* disque = CreateEntity();
		Sprite* sprite = disque->AddComponent<Sprite>("../../../Assets/ux/menu_vynile.png");
		TransformComponent* transform = disque->GetComponent<TransformComponent>();
		transform->SetPosition({ 700, 320 });
		transform->SetScale({ 0.8, 0.8 });
		disque->GetComponent<TagComponent>()->AddTag("disque");

	}

	{

		Entity* ButtonPlay = CreateEntity();
		Button* button = ButtonPlay->AddComponent<Button>();

		float angle = (3*M_PI)/2.f; 
		float x = m_center.x + m_disqueRadius * std::cos(angle);
		float y = m_center.y + m_disqueRadius * std::sin(angle);

		TransformComponent* transform = ButtonPlay->GetComponent<TransformComponent>();
		transform->SetPosition({ x, y });


		button->SetOnClickWithArg([this, ButtonPlay](Entity* entity) { Turn(ButtonPlay); });
		button->SetIsSelected(true);

		button->SetSize({ 969, 146 });
		transform->SetScale({ 0.3, 0.3 });

		Sprite* sprite = ButtonPlay->AddComponent<Sprite>("../../../Assets/ux/menu_continue.png");
		sprite->SetOffset({  484* transform->GetScale().x, 73 * transform->GetScale().x });
		ButtonPlay->GetComponent<TagComponent>()->AddTag("ResumeButton");
		
		m_buttons.push_back(Buttons{ ButtonPlay,  (3 * M_PI) / 2.f , [this]() { Resume(); } });
		ButtonPlay->GetComponent<TagComponent>()->AddTag("Button");

	} 
	
	{
		Entity* ButtonSettings = CreateEntity();
		Button* button = ButtonSettings->AddComponent<Button>();


		float angle = (11 * M_PI) / 6.f;
		float x = m_center.x + m_disqueRadius * std::cos(angle);
		float y = m_center.y + m_disqueRadius * std::sin(angle);

		TransformComponent* transform = ButtonSettings->GetComponent<TransformComponent>();
		transform->SetPosition({ x, y });


		button->SetOnClickWithArg([this, ButtonSettings](Entity* entity) { Turn(ButtonSettings); });

		button->SetSize({ 969, 146 });
		transform->SetScale({ 0.3, 0.3 });

		Sprite* sprite = ButtonSettings->AddComponent<Sprite>("../../../Assets/ux/menu_options.png");
		sprite->SetOffset({ 484 * transform->GetScale().x, 73 * transform->GetScale().x });
		ButtonSettings->GetComponent<TagComponent>()->AddTag("SettingsButton");

		m_buttons.push_back(Buttons{ ButtonSettings,  (11 * M_PI) / 6.f , [this]() { Settings(); } });
		ButtonSettings->GetComponent<TagComponent>()->AddTag("Button");

	}
	
	{
		Entity* ButtonSave = CreateEntity();
		Button* button = ButtonSave->AddComponent<Button>();



		TransformComponent* transform = ButtonSave->GetComponent<TransformComponent>();



		float angle = (M_PI) / 6.f;
		float x = m_center.x + m_disqueRadius * std::cos(angle);
		float y = (m_center.y + m_disqueRadius * std::sin(angle));

		transform->SetPosition({ x, y });

		button->SetSize({ 969, 146 });
		transform->SetScale({0.3, 0.3 });

		button->SetOnClickWithArg([this, ButtonSave](Entity* entity) { Turn(ButtonSave); });

		Sprite* sprite = ButtonSave->AddComponent<Sprite>("../../../Assets/ux/menu_save.png");

		sprite->SetOffset({ 484 * transform->GetScale().x, 73 * transform->GetScale().x });
		ButtonSave->GetComponent<TagComponent>()->AddTag("SaveButton");

		m_buttons.push_back(Buttons{ ButtonSave,  (M_PI) / 6.f, [this]() { SaveButton(); } });
		ButtonSave->GetComponent<TagComponent>()->AddTag("Button");

	}
	
	{
		Entity* ButtonExit = CreateEntity();
		Button* button = ButtonExit->AddComponent<Button>();



		TransformComponent* transform = ButtonExit->GetComponent<TransformComponent>();

		
		transform->SetScale({ 0.3, 0.3 });
		button->SetSize({ 969, 146 });


		float angle = (M_PI) / 2.f;
		float x = m_center.x + m_disqueRadius * std::cos(angle);
		float y = (m_center.y + m_disqueRadius * std::sin(angle));

		transform->SetPosition({ x, y });

		button->SetOnClickWithArg([this, ButtonExit](Entity* entity) { Turn(ButtonExit); });

		Sprite* sprite = ButtonExit->AddComponent<Sprite>("../../../Assets/ux/menu_quit.png");
		sprite->SetOffset({ 484 * transform->GetScale().x, 73 * transform->GetScale().x });

		m_buttons.push_back(Buttons{ ButtonExit,  (M_PI) / 2.f , [this]() { Menu(); } });
	
		ButtonExit->GetComponent<TagComponent>()->AddTag("Button");

	}

	{
		Entity* pochette = CreateEntity();
		Sprite* sprite = pochette->AddComponent<Sprite>("../../../Assets/ux/menu_image.png");
		TransformComponent* transform = pochette->GetComponent<TransformComponent>();
		transform->SetPosition({ 300, 350 });
		transform->SetScale({ 0.7, 0.7 });
	}

	mEntities;
	mEntitiesToAdd;

	ShaderManager::Get()->SetVisible(false);
}

void PauseScene::OnUpdate(){
	{
		m_currentAngle += (m_targetAngle - m_currentAngle) * 2.f * GameManager::Get()->GetDeltaTime();

		for (auto& item : m_buttons) {
			float angle = m_currentAngle + item.angleOffset;
			float x = m_center.x + m_disqueRadius * std::cos(angle);
			float y = m_center.y + m_disqueRadius * std::sin(angle);
			item.entity->GetComponent<TransformComponent>()->SetPosition({ x, y });

		}

		bool isAtTarget = std::abs(m_targetAngle - m_currentAngle) < 0.02f /* && std::abs(m_targetAngle) > 0.01f*/;
		if (isAtTarget && m_isTurning == true) {
			m_currentAngle = m_targetAngle;
			m_isTurning = false;
			m_buttons[id].execution();
		}



		FindEntityWithTag("disque")->GetComponent<TransformComponent>()->SetRotation(m_currentAngle * 30.f);

	}

	{
		m_cursorMovementTimer -= GameManager::Get()->GetDeltaTime();
		if (m_cursorMovementTimer <= 0) {
			m_cursorMovementTimer = 0;

			if (InputManager::Get()->GetLeftJoystick().y >= 70.f) {
				m_cursorMovementTimer = 0.2;

				std::vector<Entity*> buttons = FindAllEntitiesWithTag("Button");

				for (int i = 0; i < buttons.size(); i++) {
					Button* button = buttons[i]->GetComponent<Button>();
					if (button->IsSelected()) {
						button->SetIsSelected(false);

						if (i + 1 >= buttons.size()) {
							buttons[0]->GetComponent<Button>()->SetIsSelected(true);
						}

						else {
							buttons[i + 1]->GetComponent<Button>()->SetIsSelected(true);
						}
						break;
					}
				}
			}

			else if (InputManager::Get()->GetLeftJoystick().y <= -70.f) {
				m_cursorMovementTimer = 0.2;

				std::vector<Entity*> buttons = FindAllEntitiesWithTag("Button");

				for (int i = 0; i < buttons.size(); i++) {
					Button* button = buttons[i]->GetComponent<Button>();
					if (button->IsSelected()) {
						button->SetIsSelected(false);

						if (i - 1 < 0) {
							buttons[buttons.size() - 1]->GetComponent<Button>()->SetIsSelected(true);
						}

						else {
							buttons[i - 1]->GetComponent<Button>()->SetIsSelected(true);
						}
						break;
					}
				}
			}
		}
	}

}

void PauseScene::OnExit() {
	DestroyAllEntities();
	m_buttons.clear(); 

	m_currentAngle = 0.f;
	m_targetAngle = 0.f;

	m_isTurning = false;
	id = 0;
}