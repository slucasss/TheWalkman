#include "Scene.h"
#include "SampleScene.h"

#include "Entity.h"
#include "AllComponent.h"
#include "CollisionLogic.h"
#include "GameManager.h"

#include "StatsWindow.h"
#include "Managers/InputManager.h"

#include "Managers/ShaderManager.h"

#include "Debug.h"

#include "TilemapLoader.h"

#include <iostream>
#include "SFML/Graphics.hpp"

SampleScene::SampleScene()
{

}

void SampleScene::OnInitialize()
{
	pEntity1 = CreateEntityNow();
	pEntity1->GetComponent<TransformComponent>()->SetPosition({ 525, 100 });

	pEntity1->AddComponent<CircleCollider>(50.f);
	pEntity1->AddComponent<Rigidbody2D>();

	pEntity2 = CreateEntityNow();
	pEntity2->GetComponent<TransformComponent>()->SetPosition({ 500, 500 });
	pEntity2->AddComponent<CircleCollider>(50.f);
	pEntity1->AddComponent<CollisionLogic>();


	Entity* ground = CreateEntityNow();
	ground->GetComponent<TransformComponent>()->SetPosition({ 1000, 710 });
	ground->AddComponent<BoxCollider>(400.f, 20.f);

	
		//Sprite entity
		Entity* image = CreateEntityNow();
		TransformComponent* transform = image->GetComponent<TransformComponent>();
		transform->SetPosition({ 0, 0 });

		//transform->SetRotation(30);
		//transform->SetScale({ 0.3, 0.3 });
		//transform->SetFlip(Flip::Both);

		image->GetComponent<TagComponent>()->AddTag("image1");

		Sprite* sprite = image->AddComponent<Sprite>("../../../src/oui.png");

		//ShaderManager::Get()->ApplyTo(sprite, "../../../src/Shaders/HologrammeShader.frag");



		Entity* image2 = CreateEntityNow();
		TransformComponent* transform2 = image2->GetComponent<TransformComponent>();
		transform2->SetPosition({ 900, 100 });
		transform2->SetScale({ 0.3, 0.3 });

		image2->GetComponent<TagComponent>()->AddTag("image2");

		Sprite* sprite2 = image2->AddComponent<Sprite>("../../../src/oui.png");


		ShaderManager::Get()->ApplyTo(sprite2, "../../../src/Shaders/GlitchShader.frag");
		
	

	{
		//Animation entity
		Entity* entity = CreateEntityNow();
		TransformComponent* transform2 = entity->GetComponent<TransformComponent>();
		transform2->SetPosition({ 500, 100 });
		transform2->SetScale({ 3, 3 });

		entity->GetComponent<TagComponent>()->AddTag("animation");

		Sprite* sprite2 = entity->AddComponent<Sprite>("../../../src/SpriteSheet.png");
		sprite2->SetSrcRect(0, 0, 20, 35);

		Animation* animation = entity->AddComponent<Animation>("../../../src/animation.json");
		animation->SetAnimation("idle");

		entity->AddComponent<Listener>();

		test = GameManager::Get()->CreateStatsWindow("animation");

		test->Inspect(entity);
		
		//ShaderManager::Get()->SetCurrentMainShader("../../../src/Shaders/AmbiantShader.frag");

		


		//Camera entity linked with the animation
		Entity* camera = CreateEntityNow();

		Camera* cam = camera->AddComponent<Camera>();
		camera->GetComponent<TransformComponent>()->SetPosition({ 0, 0 });

		cam->SetLink(entity);
		cam->SetSmooth(true);


		cam->SetMainCamera();
		//cam->TransitionTo(sf::FloatRect{ 500, 500, 600, 300 }, 400, 400);
		//cam->TransitionTo(entity, { -1, -1 }, 200, 200);


		test = GameManager::Get()->CreateStatsWindow("camera");
		test->Inspect(camera);


		sf::View view = GameManager::Get()->GetWindow()->getView();

		//camera->GetComponent<TagComponent>()->AddTag("camera1");
	}



	ShaderManager::Get()->SetVisible(true);

	srand(NULL);


}

void SampleScene::OnUpdate()
{
	//std::cout << f << std::endl;
	 

	/*std::cout << "LeftJoystick X " << InputManager::Get()->GetLeftJoystick().x << std::endl;
	std::cout << "LeftJoystick Y " << InputManager::Get()->GetLeftJoystick().y << std::endl;*/

	//std::cout << "RightJoystick X " << InputManager::Get()->GetRightJoystick().x << std::endl;
	//std::cout << "RightJoystick Y " << InputManager::Get()->GetRightJoystick().y << std::endl;


	/*if(InputManager::Get()->IsHeld(sf::Keyboard::A)) {
		std::cout << "A Appuyé" << std::endl;
	}
	if(InputManager::Get()->IsDown(sf::Keyboard::A)) {
		std::cout << "A Cliqué" << std::endl;
	}
	if(InputManager::Get()->IsUp(sf::Keyboard::A)) {
		std::cout << "A Relevé" << std::endl;
	}*/


	//if (InputManager::Get()->IsDown(sf::Mouse::Left)) {
	//	std::cout << "souris cliqué" << std::endl;
	//}
	//else if (InputManager::Get()->IsHeld(sf::Mouse::Left)) {
	//	std::cout << "souris Appuyé" << std::endl;
	//}
	//else if (InputManager::Get()->IsUp(sf::Mouse::Left)) {
	//	std::cout << "souris Relevé" << std::endl;
	//}	

	/*std::cout << "Mouse X " << InputManager::Get()->GetMousePosition().x << std::endl;
	std::cout << "Mouse Y " << InputManager::Get()->GetMousePosition().y << std::endl;
	*/

	//if (InputManager::Get()->IsDown(1)) {
	//	std::cout << "Bouton A cliqué" << std::endl;
	//}
	//if (InputManager::Get()->IsHeld(1)) {
	//	std::cout << "Bouton A Appuyé" << std::endl;
	//}
	//if (InputManager::Get()->IsUp(1)) {
	//	std::cout << "Bouton A Relevé" << std::endl;
	//}

	Animation* animation = FindEntityWithTag("animation")->GetComponent<Animation>();
	if(InputManager::Get()->IsDown(sf::Keyboard::A)) {
		animation->SetAnimation("hit");
	}
	if (animation->IsAnimationEnded() == true && animation->AnimationIs() == "hit"){
		animation->SetAnimation("idle");
	}

	//if (InputManager::Get()->IsDown(sf::Keyboard::S)) {
	//	FindEntityWithTag("Mixer1")->GetComponent<Sound>()->Play();
	//}

	if (InputManager::Get()->IsHeld(sf::Keyboard::D)) {
		TransformComponent* transform = FindEntityWithTag("animation")->GetComponent<TransformComponent>();
		transform->Translate({ 10, 0 });
	}



	if (InputManager::Get()->IsHeld(sf::Keyboard::X)) {
		FindEntityWithTag("camera1")->GetComponent<Camera>()->TransitionTo(sf::FloatRect{ 400, 0, 640, 360 }, 100, 100);
	}
	//FindEntityWithTag("camera1")->GetComponent<Camera>()->TransitionTo(sf::FloatRect{ 0, 0, 680, 310 }, 200, 100);

	//FindEntityWithTag("camera1")->GetComponent<Camera>()->Rotate(1);




	if (InputManager::Get()->IsUp(sf::Keyboard::C)) {
		FindEntityWithTag("camera1")->GetComponent<Camera>()->FadeOut(3);
	}

	if (InputManager::Get()->IsHeld(sf::Keyboard::C)) {
		FindEntityWithTag("camera1")->GetComponent<Camera>()->FadeIn(3);
	}


	if (InputManager::Get()->IsDown(sf::Keyboard::R)) {
		ShaderManager::Get()->SetVisible(!ShaderManager::Get()->IsApply());
	}


	ShaderManager::Get()->SetParameter<float>("../../../src/Shaders/PulseShader.frag", "beatTime", m_BeatTime);
	if (InputManager::Get()->IsDown(sf::Keyboard::P)) {
		m_BeatTime = GameManager::Get()->GetShaderClock().getElapsedTime().asSeconds();
		ShaderManager::Get()->SetParameter<float>("../../../src/Shaders/PulseShader.frag", "beatTime", m_BeatTime);
	}


	sf::Vector2f pos = FindEntityWithTag("animation")->GetComponent<TransformComponent>()->GetPosition();
	sf::Vector2i screenPos = GameManager::Get()->GetWindow()->mapCoordsToPixel(pos, GameManager::Get()->GetSceneTexture()->getView());
	ShaderManager::Get()->SetParameter<sf::Glsl::Vec4>("../../../src/Shaders/AmbiantShader.frag", "lights[0]", sf::Glsl::Vec4(screenPos.x, screenPos.y, 100.0,  1.0));


	sf::Vector2i screenPos2 = GameManager::Get()->GetWindow()->mapCoordsToPixel(sf::Vector2f{000, -200}, GameManager::Get()->GetSceneTexture()->getView());
	ShaderManager::Get()->SetParameter<sf::Glsl::Vec4>("../../../src/Shaders/AmbiantShader.frag", "cones[0]", sf::Glsl::Vec4(screenPos2.x, screenPos2.y, 300, 2.0));
	ShaderManager::Get()->SetParameter<sf::Glsl::Vec4>("../../../src/Shaders/AmbiantShader.frag", "conesDirs[0]", sf::Glsl::Vec4(0 , -1 , 0.3 , 0.0 ));
}