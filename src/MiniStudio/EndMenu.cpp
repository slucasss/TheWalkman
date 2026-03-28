#include "EndMenu.h"
#include "SceneManager.h"
#include "GameManager.h"

#include "Managers/ShaderManager.h"
#include "Managers/InputManager.h"

#include "Button.h"
#include "Sprite.h"
#include "TransformComponent.h"
#include "TagComponent.h"

namespace {
	void ReturnMenu() {
		SceneManager::GetInstance().ChangeScene("MainMenu");
		SceneManager::GetInstance().FindSceneById("MainMenu")->SetInitalize(true);
	}
}




void EndMenu::OnInitialize() {
	GameManager::Get()->mClearColor = sf::Color::Black;
	GameManager::Get()->GetSceneTexture()->setView(GameManager::Get()->GetSceneTexture()->getDefaultView());

	ShaderManager::Get()->SetVisible(false);

	

	{
		Entity* background = CreateEntityNow();
		Sprite* sprite = background->AddComponent<Sprite>("../../../Assets/Boss/wall_boss.png");
		sprite->SetUI(true);

		TransformComponent* transform = background->GetComponent<TransformComponent>();
	

		transform->SetPosition({ 640, 340 });
		transform->SetScale(0.7, 0.7);
	}
	{
		Entity* background = CreateEntityNow();
		Sprite* sprite = background->AddComponent<Sprite>("../../../Assets/Boss/Posters_boss.png");
		sprite->SetUI(true);

		TransformComponent* transform = background->GetComponent<TransformComponent>();
	

		transform->SetPosition({ 0, 300 });
		transform->SetScale(1.f, 1.f);
	}


	{
		Entity* entity = CreateEntityNow();
		Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/ux/text_death_screen.png");
		sprite->SetUI(true);

		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		transform->SetPosition({ 650, 50 });
		transform->SetScale(0.7, 0.7);
	}


	{
		Entity* entity = CreateEntityNow();
		Sprite* sprite = nullptr;

		if (m_endType== "loose") {
			sprite = entity->AddComponent<Sprite>("../../../Assets/Boss/punk_win.png");
		}
		else if(m_endType == "win"){
			sprite = entity->AddComponent<Sprite>("../../../Assets/Boss/punk_loose.png");
		}



		sprite->SetUI(true);

		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		entity->GetComponent<TagComponent>()->AddTag("BossPose");


		transform->SetPosition({ 600, 360 });
		transform->SetScale(1.f, 1.f);
	}


	//{
	//	Entity* entity = CreateEntity();
	//	Sprite* sprite = entity->AddComponent<Sprite>("../../../Assets/Boss/lights_boss.png");

	//	sprite->SetUI(true);

	//	TransformComponent* transform = entity->GetComponent<TransformComponent>();


	//	transform->SetPosition({ 200.f, 200.f });
	//	transform->SetScale(1.f, 1.f);


	//	//transform->SetRotation(180.f);
	//
	//}

	


	{
		Entity* ButtonExit = CreateEntityNow();
		Button* button = ButtonExit->AddComponent<Button>();


		button->SetOnClick([this]() { ReturnMenu(); });
		button->SetSize({ 442, 145 });

		TransformComponent* transform = ButtonExit->GetComponent<TransformComponent>();
		transform->SetPosition({ 500, 550 });
		transform->SetScale({ 0.7, 0.7 });

		Sprite* sprite = ButtonExit->AddComponent<Sprite>("../../../Assets/ux/quit_death_screen.png");
		sprite->SetOffset({ 221 * transform->GetScale().x, 72 * transform->GetScale().x });
		sprite->SetUI(true);



		ButtonExit->GetComponent<TagComponent>()->AddTag("Button");
	}

}


void EndMenu::OnLoading()
{
}

void EndMenu::OnUpdate(){

}

void EndMenu::OnExit()
{
}

void EndMenu::SetEndType(const std::string& type){
	m_endType = type;
}
