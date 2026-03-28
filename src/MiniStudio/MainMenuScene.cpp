#include "MainMenuScene.h"
#include "GameManager.h"
#include "Managers/AssetsManager.h"
#include "Entity.h"
#include "Button.h"
#include "SceneManager.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "Managers/InputManager.h"

#include "Sprite.h"
#include <iostream>


void Play() {
	SceneManager::GetInstance().ChangeScene("Start");
	SceneManager::GetInstance().FindSceneById("Start")->SetInitalize(true);
}

void End() {
	GameManager::Get()->GetWindow()->close();
}

void Settings() {
	SceneManager::GetInstance().ChangeScene("Settings");
	SceneManager::GetInstance().FindSceneById("Settings")->SetInitalize(true);
}


void MainMenuScene::OnInitialize(){

	GameManager::Get()->GetSceneTexture()->setView(GameManager::Get()->GetSceneTexture()->getDefaultView());

	GameManager::Get()->mClearColor = sf::Color{ 246, 236, 215, 0 };
	
	ShaderManager::Get()->SetCurrentMainShader("../../../src/Shaders/MainMenuShader.frag");
	ShaderManager::Get()->SetVisible(true);

	{
		Entity* Background = CreateEntity();

		TransformComponent* transform = Background->GetComponent<TransformComponent>();
		transform->SetPosition({ 640, 310 });
		transform->SetScale({ 2, 2 });

		Sprite* sprite = Background->AddComponent<Sprite>("../../../Assets/BackgroundMainMenu.png");
	}

	{
		Entity* logo = CreateEntity();

		TransformComponent* transform = logo->GetComponent<TransformComponent>();
		transform->SetPosition({ 625, 180 });
		transform->SetScale({ 1, 1 });

		Sprite* sprite = logo->AddComponent<Sprite>("../../../Assets/ux/Logo_text.png");

	}
	
	{
		Entity* ButtonPlay = CreateEntity();
		Button* button = ButtonPlay->AddComponent<Button>();
		if (InputManager::Get()->IsControllerConnected()) {
			button->SetIsSelected(true);
		}

		button->SetOnClick([this]() { Play(); });
		button->SetSize({ 600, 157 });
		TransformComponent* transform = ButtonPlay->GetComponent<TransformComponent>();
		transform->SetPosition({ 425, 350 });
		transform->SetScale({ 0.7, 0.7 });

		Sprite* sprite = ButtonPlay->AddComponent<Sprite>("../../../Assets/ux/start_not.png");

		button->SetPath("../../../Assets/ux/start_not.png");
		button->SetSelectedPath("../../../Assets/ux/start_yes.png");


		sprite->SetOffset({ 300 * transform->GetScale().x, 78 * transform->GetScale().x });
		sprite->SetSrcRect(12, 10, 600, 157);

		ButtonPlay->GetComponent<TagComponent>()->AddTag("Button");

	}

	{
		Entity* ButtonSettings = CreateEntity();
		Button* button = ButtonSettings->AddComponent<Button>();


		button->SetOnClick([this]() { Settings(); });
		button->SetSize({ 600, 157 });

		TransformComponent* transform = ButtonSettings->GetComponent<TransformComponent>();
		transform->SetPosition({ 425, 475 });
		transform->SetScale({ 0.7, 0.7 });

		Sprite* sprite = ButtonSettings->AddComponent<Sprite>("../../../Assets/ux/options_not.png");
		sprite->SetOffset({ 300 * transform->GetScale().x, 78 * transform->GetScale().x });
		sprite->SetSrcRect(12, 10, 600, 157);

		button->SetPath("../../../Assets/ux/options_not.png");
		button->SetSelectedPath("../../../Assets/ux/options_yes.png");

		ButtonSettings->GetComponent<TagComponent>()->AddTag("Button");

	}

	{
		Entity* ButtonExit = CreateEntity();
		Button* button = ButtonExit->AddComponent<Button>();


		button->SetOnClick([this]() { End(); });
		button->SetSize({ 600, 157 });

		TransformComponent* transform = ButtonExit->GetComponent<TransformComponent>();
		transform->SetPosition({ 425, 600 });
		transform->SetScale({ 0.7, 0.7 });

		Sprite* sprite = ButtonExit->AddComponent<Sprite>("../../../Assets/ux/quit_not.png");
		sprite->SetOffset({ 300 * transform->GetScale().x, 78 * transform->GetScale().x });
		sprite->SetSrcRect(12, 10, 600, 157);

		button->SetPath("../../../Assets/ux/quit_not.png");
		button->SetSelectedPath("../../../Assets/ux/quit_yes.png");

		ButtonExit->GetComponent<TagComponent>()->AddTag("Button");
	}
}

void MainMenuScene::OnLoading() {
	sf::RenderWindow* window = GameManager::Get()->GetWindow();
	while (alpha != 255) {

		window->clear();

		alpha = alpha + 1;
		if (alpha > 255) {
			alpha = 255;
		}


		sf::Sprite sprite;
		sprite.setTexture(*AssetsManager::Get()->Load("../../../Assets/logo.png"));
		sprite.setScale({0.5, 0.5});
		sprite.setPosition({ 170, 50 });
		window->draw(sprite);


		sf::RectangleShape fade(sf::Vector2f(window->getSize()));
		fade.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(alpha)));
		window->draw(fade);


		window->display();
	}
}

void MainMenuScene::OnUpdate(){
	if (InputManager::Get()->IsControllerConnected()) {

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

						if (i+1 >= buttons.size()) {
							buttons[0]->GetComponent<Button>()->SetIsSelected(true);
						}

						else {
							buttons[i+1]->GetComponent<Button>()->SetIsSelected(true);
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

						if (i-1 < 0) {
							buttons[buttons.size()-1]->GetComponent<Button>()->SetIsSelected(true);
						}

						else {
							buttons[i-1]->GetComponent<Button>()->SetIsSelected(true);
						}
						break;
					}
				}
			}
		}
	}
	
	
}

void MainMenuScene::OnExit() {
	DestroyAllEntities();
	ShaderManager::Get()->SetVisible(false);
}