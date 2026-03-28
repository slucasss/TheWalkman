#include "DeathMenuScene.h"
#include "Managers/ShaderManager.h"

#include "Entity.h"
#include "TransformComponent.h"
#include "Button.h"
#include "TagComponent.h"

#include "Sprite.h"

#include "Managers/InputManager.h"
#include "GameManager.h"
#include "SceneManager.h"


void ReloadSave() {
	SceneManager::GetInstance().ChangeScene("Start");
	SceneManager::GetInstance().FindSceneById("Start")->Load("../../../Assets/saveTest.json");
}

void ReturnMenu() {
	SceneManager::GetInstance().ChangeScene("MainMenu");
	SceneManager::GetInstance().FindSceneById("Start")->DestroyAllEntities();
	SceneManager::GetInstance().FindSceneById("Start")->SetInitalize(true);
	SceneManager::GetInstance().FindSceneById("MainMenu")->SetInitalize(true);
}


void DeathMenuScene::OnInitialize(){
	GameManager::Get()->mClearColor = sf::Color::Black;
	GameManager::Get()->GetSceneTexture()->setView(GameManager::Get()->GetSceneTexture()->getDefaultView());

	ShaderManager::Get()->SetVisible(false);

	{
		Entity* entity = CreateEntity();
		Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/death_screen_left.png");
		sprite->SetUI(true);

		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		transform->SetPosition({ 280, 360 });
		transform->SetScale(0.7, 0.7);
	}

	{
		Entity* entity = CreateEntity();
		Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/death_screen_right.png");
		sprite->SetUI(true);

		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		transform->SetPosition({ 1065, 360 });
		transform->SetScale(0.7, 0.7);
	}

	{
		Entity* entity = CreateEntity();
		Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/text_death_screen.png");
		sprite->SetUI(true);

		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		transform->SetPosition({ 650, 150 });
		transform->SetScale(0.7, 0.7);
	}




	//{
	//	Entity* ButtonPlay = CreateEntity();
	//	Button* button = ButtonPlay->AddComponent<Button>();
	//	if (InputManager::Get()->IsControllerConnected()) {
	//		button->SetIsSelected(true);
	//	}

	//	button->SetOnClick([this]() { ReloadSave(); });
	//	button->SetSize({ 442, 145 });
	//	TransformComponent* transform = ButtonPlay->GetComponent<TransformComponent>();
	//	transform->SetPosition({ 500, 400 });
	//	transform->SetScale({ 0.7, 0.7 });

	//	Sprite* sprite = ButtonPlay->AddComponent<Sprite>("../../../Assets/ux/retry_death_screen.png");
	//	sprite->SetUI(true);


	//	sprite->SetOffset({ 221 * transform->GetScale().x, 72 * transform->GetScale().x });

	//	ButtonPlay->GetComponent<TagComponent>()->AddTag("Button");

	//}

	{
		Entity* ButtonExit = CreateEntity();
		Button* button = ButtonExit->AddComponent<Button>();


		button->SetOnClick([this]() { ReturnMenu(); });
		button->SetSize({ 442, 145 });
		if (InputManager::Get()->IsControllerConnected()) {
			button->SetIsSelected(true);
		}

		TransformComponent* transform = ButtonExit->GetComponent<TransformComponent>();
		transform->SetPosition({ 500, 550 });
		transform->SetScale({ 0.7, 0.7 });

		Sprite* sprite = ButtonExit->AddComponent<Sprite>("../../../Assets/ux/quit_death_screen.png");
		sprite->SetOffset({ 221 * transform->GetScale().x, 72 * transform->GetScale().x });
		sprite->SetUI(true);

		

		ButtonExit->GetComponent<TagComponent>()->AddTag("Button");
	}

}

void DeathMenuScene::OnLoading(){

}

void DeathMenuScene::OnUpdate(){
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

void DeathMenuScene::OnExit(){
	DestroyAllEntities();
}
