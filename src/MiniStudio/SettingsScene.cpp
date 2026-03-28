#include "SettingsScene.h"

#include "RebindButton.h"
#include "KeyBinds.h"
#include "SettingsLoader.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "TagComponent.h"

#include "Sprite.h"
#include "TextComponent.h"

#include "GameManager.h"
#include "Managers/InputManager.h"
#include "SceneManager.h"


namespace {

	void Return() {
		SceneManager::GetInstance().ChangeToPreviousScene();
	}

	void ApplyModifications() {
		KeyBinds::Get()->Save();
	}
}

void SettingsScene::OnInitialize(){
	

	//Title
	{
		Entity* title = CreateEntity();


		TransformComponent* transform = title->GetComponent<TransformComponent>();
		transform->SetPosition({ 250, 0 });

		TextComponent* text = title->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 50);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText("Keyboard");
	}
	{
		Entity* title = CreateEntity();


		TransformComponent* transform = title->GetComponent<TransformComponent>();
		transform->SetPosition({ 550, 0 });

		TextComponent* text = title->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 50);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText("Controller");
	}


	{
		Entity* title = CreateEntity();
		

		TransformComponent* transform = title->GetComponent<TransformComponent>();
		transform->SetPosition({ 30, 100 });

		TextComponent* text = title->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 50);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText("Go right");
	}
	{
		Entity* title = CreateEntity();
		

		TransformComponent* transform = title->GetComponent<TransformComponent>();
		transform->SetPosition({ 30, 200 });

		TextComponent* text = title->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 50);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText("Go left");
	}
	{
		Entity* title = CreateEntity();
		

		TransformComponent* transform = title->GetComponent<TransformComponent>();
		transform->SetPosition({ 30, 300 });

		TextComponent* text = title->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 50);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText("Jump");
	}
	{
		Entity* title = CreateEntity();
		

		TransformComponent* transform = title->GetComponent<TransformComponent>();
		transform->SetPosition({ 30, 400 });

		TextComponent* text = title->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 50);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText("Run");
	}
	{
		Entity* title = CreateEntity();
		

		TransformComponent* transform = title->GetComponent<TransformComponent>();
		transform->SetPosition({ 30, 500 });

		TextComponent* text = title->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 50);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText("Pause");
	}
	{
		Entity* title = CreateEntity();
		

		TransformComponent* transform = title->GetComponent<TransformComponent>();
		transform->SetPosition({ 30, 600 });

		TextComponent* text = title->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 40);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText("Change music");
	}

	//Keyboard buttons
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("GoLeft");
		rebind->SetSize({252, 158});
		rebind->SetButtonType(1);
		rebind->SetIsSelected(false);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({300, 100});
		transform->SetScale({0.5, 0.5});

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		TextComponent* text = button->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 40);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText(KeyBinds::Get()->KeyToString(KeyBinds::Get()->GetKeyboardBind("GoLeft")));
		text->SetOffset({ 45, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");

	}
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("GoRight");
		rebind->SetSize({252, 158});
		rebind->SetButtonType(1);
		rebind->SetIsSelected(false);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({300, 200});
		transform->SetScale({0.5, 0.5});

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		TextComponent* text = button->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 40);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText(KeyBinds::Get()->KeyToString(KeyBinds::Get()->GetKeyboardBind("GoRight")));
		text->SetOffset({ 45, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");

	}
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("Jump");
		rebind->SetSize({252, 158});
		rebind->SetButtonType(1);
		rebind->SetIsSelected(false);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({300, 300});
		transform->SetScale({0.5, 0.5});

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		TextComponent* text = button->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 40);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText(KeyBinds::Get()->KeyToString(KeyBinds::Get()->GetKeyboardBind("Jump")));
		text->SetOffset({ 20, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");

	}
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("Run");
		rebind->SetSize({252, 158});
		rebind->SetButtonType(1);
		rebind->SetIsSelected(false);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({300, 400});
		transform->SetScale({0.5, 0.5});

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		TextComponent* text = button->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 40);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText(KeyBinds::Get()->KeyToString(KeyBinds::Get()->GetKeyboardBind("Run")));
		text->SetOffset({ 20, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");

	}
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("Pause");
		rebind->SetSize({252, 158});
		rebind->SetButtonType(1);
		rebind->SetIsSelected(false);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({300, 500});
		transform->SetScale({0.5, 0.5});

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		TextComponent* text = button->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 40);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText(KeyBinds::Get()->KeyToString(KeyBinds::Get()->GetKeyboardBind("Pause")));
		text->SetOffset({ 10, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");

	}
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("MusicChange");
		rebind->SetSize({252, 158});
		rebind->SetButtonType(1);
		rebind->SetIsSelected(false);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({300, 600});
		transform->SetScale({0.5, 0.5});

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		TextComponent* text = button->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 40);
		text->SetDrawColor(0, 0, 0, 255);
		text->SetText(KeyBinds::Get()->KeyToString(KeyBinds::Get()->GetKeyboardBind("MusicChange")));
		text->SetOffset({ 45, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");

	}

	//Controller buttons
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("Jump");
		rebind->SetSize({ 252, 158 });
		rebind->SetButtonType(2);
		rebind->SetIsSelected(true);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({ 600, 300 });
		transform->SetScale({ 0.5, 0.5 });

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		Sprite* sprite2 = button->AddComponent<Sprite>("../../../Assets/XboxButtons.png");
		sprite2->SetOffset({ 45, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");
	}
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("Pause");
		rebind->SetSize({ 252, 158 });
		rebind->SetButtonType(2);
		rebind->SetIsSelected(false);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({ 600, 500 });
		transform->SetScale({ 0.5, 0.5 });

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		Sprite* sprite2 = button->AddComponent<Sprite>("../../../Assets/XboxButtons.png");
		sprite2->SetOffset({ 45, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");
	}
	{
		Entity* button = CreateEntity();
		RebindButton* rebind = button->AddComponent<RebindButton>("MusicChange");
		rebind->SetSize({ 252, 158 });
		rebind->SetButtonType(2);
		rebind->SetIsSelected(false);

		TransformComponent* transform = button->GetComponent<TransformComponent>();
		transform->SetPosition({ 600, 600 });
		transform->SetScale({ 0.5, 0.5 });

		Sprite* sprite = button->AddComponent<Sprite>("../../../Assets/EmptyButton.png");
		sprite->SetOffset({ 126 * transform->GetScale().x, 78 * transform->GetScale().x });

		Sprite* sprite2 = button->AddComponent<Sprite>("../../../Assets/XboxButtons.png");
		sprite2->SetOffset({ 45, 10 });

		button->GetComponent<TagComponent>()->AddTag("button");
	}


	{
		Entity* ButtonExit = CreateEntity();
		Button* button = ButtonExit->AddComponent<Button>();


		button->SetOnClick([this]() { ApplyModifications(); });
		button->SetSize({ 600, 157 });

		TransformComponent* transform = ButtonExit->GetComponent<TransformComponent>();
		transform->SetPosition({ 800, 600 });
		transform->SetScale({ 0.35, 0.35 });

		Sprite* sprite = ButtonExit->AddComponent<Sprite>("../../../Assets/ux/saves_not.png");
		sprite->SetOffset({ 300 * transform->GetScale().x, 78 * transform->GetScale().x });

		sprite->SetSrcRect(12, 10, 600, 157);

		button->SetPath("../../../Assets/ux/saves_not.png");
		button->SetSelectedPath("../../../Assets/ux/saves_yes.png");

		ButtonExit->GetComponent<TagComponent>()->AddTag("button");
	}

	{
		Entity* ButtonExit = CreateEntity();
		Button* button = ButtonExit->AddComponent<Button>();


		button->SetOnClick([this]() { Return(); });
		button->SetSize({ 600, 157 });

		TransformComponent* transform = ButtonExit->GetComponent<TransformComponent>();
		transform->SetPosition({ 1050, 600 });
		transform->SetScale({ 0.35, 0.35 });



		Sprite* sprite = ButtonExit->AddComponent<Sprite>("../../../Assets/ux/quit_not.png");
		sprite->SetOffset({ 300 * transform->GetScale().x, 78 * transform->GetScale().x });
		//{12, 10}  {12, 12}
		
		sprite->SetSrcRect(12, 10, 600, 157);
		button->SetPath("../../../Assets/ux/quit_not.png");
		button->SetSelectedPath("../../../Assets/ux/quit_yes.png");

		ButtonExit->GetComponent<TagComponent>()->AddTag("button");
	}

}

void SettingsScene::OnLoading(){

}

void SettingsScene::OnUpdate(){

	if (InputManager::Get()->IsControllerConnected()) {
		m_cursorMovementTimer -= GameManager::Get()->GetDeltaTime();
		if (m_cursorMovementTimer <= 0) {
			m_cursorMovementTimer = 0;

			if (InputManager::Get()->GetLeftJoystick().y >= 70.f) {
				m_cursorMovementTimer = 0.2;

				std::vector<Entity*> buttons = FindAllEntitiesWithTag("button");

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

				std::vector<Entity*> buttons = FindAllEntitiesWithTag("button");

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

void SettingsScene::OnExit(){
	DestroyAllEntities();
}
