#include "Scene.h"
#include "StartScene.h"

#include "Entity.h"
#include "AllComponent.h"
#include "GameManager.h"
#include "Managers/AssetsManager.h"

#include "StatsWindow.h"
#include "Managers/InputManager.h"

#include "Debug.h"

#include "TilemapLoader.h"
#include "TiledUtils.h"
#include "PropRenderer.h"
#include "PropAtlas.h"

#include <iostream>
#include "PlayerMovement.h"
#include "PlayerWalljump.h"
#include "PlayerHealth.h"
#include "MusicChange.h"
#include "PlayerInteract.h"
#include "Inventory.h"
#include "ItemData.h"
#include "PlayerAnimationController.h"

#include "HealthBar.h"
#include "DoorLogic.h"
#include "RatMovement.h"
#include "BatMovement.h"

#include "KeyBinds.h"
#include "SceneManager.h"

#include "AmbiantShader.h"
#include "HypeBar.h"
#include "CheckZoneLogic.h"
#include "RhythmSpawner.h"
#include "SoundEffect.h"


#include "Rigidbody2D.h"

#include "Button.h"


namespace {
	void ReturnMenu() {
		SceneManager::GetInstance().ChangeScene("MainMenu");
		SceneManager::GetInstance().FindSceneById("MainMenu")->SetInitalize(true);
	}
}

StartScene::StartScene()
{
	atlas = new PropAtlas;
}

StartScene::~StartScene()
{
	delete atlas;
}

struct PropInfo
{
	std::string source;
	float x;
	float y;
	float w;
	float h;
	float rotation;
	Flip flip;
};

PropInfo GetPropInfo(XMLElement* obj, sf::Vector2f scale)
{
	int gid = obj->IntAttribute("gid");

	bool flipH = gid & 0x80000000;
	bool flipV = gid & 0x40000000;

	gid &= ~(0x80000000 | 0x40000000);

	std::string source = TiledUtils::ResolveTileGID("../../../Assets/mapTest.tmx", gid);

	PropInfo info;

	info.source = source;
	info.x = obj->FloatAttribute("x") * scale.x;
	info.y = obj->FloatAttribute("y") * scale.y;
	info.w = obj->FloatAttribute("width") * scale.x;
	info.h = obj->FloatAttribute("height") * scale.y;
	info.rotation = obj->FloatAttribute("rotation");

	if (flipH && flipV)
		info.flip = Flip::Both;
	else if (flipH)
		info.flip = Flip::Horizontal;
	else if (flipV)
		info.flip = Flip::Vertical;
	else
		info.flip = Flip::None;

	return info;
}

void StartScene::OnInitialize() {

	std::vector<PropInfo> propsFrontPlayer;
	std::vector<PropInfo> propsFront;
	std::vector<PropInfo> propsBack;

	m_loader->AddObjectLayerHandler("DecorFrontPlayer", [&](XMLElement* obj, Scene*, sf::Vector2f scale)
		{
			PropInfo info = GetPropInfo(obj, scale);
			propsFrontPlayer.push_back(info);
			atlas->AddImage(info.source);
		});

	m_loader->AddObjectLayerHandler("DecorFront", [&](XMLElement* obj, Scene*, sf::Vector2f scale)
		{
			PropInfo info = GetPropInfo(obj, scale);
			propsFront.push_back(info);
			atlas->AddImage(info.source);
		});

	m_loader->AddObjectLayerHandler("DecorBack", [&](XMLElement* obj, Scene*, sf::Vector2f scale)
		{
			PropInfo info = GetPropInfo(obj, scale);
			propsBack.push_back(info);
			atlas->AddImage(info.source);
		});

	m_loader->AddObjectLayerHandler("Light", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;

			float radius = TiledUtils::GetProperty<float>(obj, "Radius", 200.f);
			float intensity = TiledUtils::GetProperty<float>(obj, "Intensity", 1.f);
			int r = TiledUtils::GetProperty<int>(obj, "R", 255);
			int g = TiledUtils::GetProperty<int>(obj, "G", 255);
			int b = TiledUtils::GetProperty<int>(obj, "B", 255);

			AmbiantShader::Get()->AddLight({ x, y, }, radius, intensity, { r / 255.f, g / 255.f, b / 255.f });
		});

	m_loader->AddObjectLayerHandler("ConeLight", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;

			float radius = TiledUtils::GetProperty<float>(obj, "Radius", 200.f);
			float intensity = TiledUtils::GetProperty<float>(obj, "Intensity", 1.f);
			int r = TiledUtils::GetProperty<int>(obj, "R", 255);
			int g = TiledUtils::GetProperty<int>(obj, "G", 255);
			int b = TiledUtils::GetProperty<int>(obj, "B", 255);
			float dirX = TiledUtils::GetProperty<float>(obj, "DirX", 0.f);
			float dirY = TiledUtils::GetProperty<float>(obj, "DirY", -1.f);
			float angle = TiledUtils::GetProperty<float>(obj, "Angle", 0.523598f);

			AmbiantShader::Get()->AddConeLight({ x, y, }, radius, intensity, { r / 255.f, g / 255.f, b / 255.f }, { dirX, dirY }, angle);
		});

	m_loader->AddObjectLayerHandler("Ground", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;
			float w = obj->FloatAttribute("width") * scale.x;
			float h = obj->FloatAttribute("height") * scale.y;

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "");
			std::string monde = TiledUtils::GetProperty<std::string>(obj, "Monde", "");

			Entity* collider = scene->CreateEntityNow();
			TransformComponent* transform = collider->GetComponent<TransformComponent>();
			collider->GetComponent<RenderLayerComponent>()->SetLayer(1);

			transform->SetPosition({ x + w * 0.5f, y + h * 0.5f });

			collider->AddComponent<BoxCollider>(w, h)->SetVisible(false);
			TagComponent* tc = collider->GetComponent<TagComponent>();
			tc->AddTag("Ground");
			if (tag != "")
				tc->AddTag(tag);
			if (monde != "")
				tc->AddTag(monde);
		});

	m_loader->AddObjectLayerHandler("DamageZone", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;
			float w = obj->FloatAttribute("width") * scale.x;
			float h = obj->FloatAttribute("height") * scale.y;

			std::string monde = TiledUtils::GetProperty<std::string>(obj, "Monde", "");
			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "");

			Entity* collider = scene->CreateEntityNow();
			TransformComponent* transform = collider->GetComponent<TransformComponent>();
			collider->GetComponent<RenderLayerComponent>()->SetLayer(1);

			transform->SetPosition({ x + w * 0.5f, y + h * 0.5f });

			collider->AddComponent<BoxCollider>(w, h);

			TagComponent* tc = collider->GetComponent<TagComponent>();
			tc->AddTag("DamageZone");
			if (monde != "")
				tc->AddTag(monde);
			if (tag != "")
				tc->AddTag(tag);

			collider->GetComponent<BoxCollider>()->SetTrigger(true);
		});

	m_loader->AddObjectLayerHandler("Platforms", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			PropInfo info = GetPropInfo(obj, scale);

			std::string type = TiledUtils::GetProperty<std::string>(obj, "Type", "");
			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "");

			Entity* platform = scene->CreateEntityNow();
			platform->GetComponent<RenderLayerComponent>()->SetLayer(1);

			TagComponent* tc = platform->GetComponent<TagComponent>();
			tc->AddTag("Platform");
			if (type != "")
				tc->AddTag(type);
			if (tag != "")
				tc->AddTag(tag);

			Sprite* sprite = platform->AddComponent<Sprite>(info.source);

			TransformComponent* transform = platform->GetComponent<TransformComponent>();
			transform->SetPosition({ info.x + info.w * 0.5f, info.y - info.h * 0.5f });
			transform->SetRotation(info.rotation);


			if (type == "ecroulement")
			{
				transform->SetScale({ info.w / sprite->GetWidth() * 3, info.h / sprite->GetHeight() });
			}
			else if (type == "Acid")
			{
				transform->SetScale({ info.w / sprite->GetWidth() * 6, info.h / sprite->GetHeight() });
				platform->AddComponent<Animation>("../../../Assets/waterAnim.json")->SetAnimation("default");
			}
			else
			{
				transform->SetScale({ info.w / sprite->GetWidth(), info.h / sprite->GetHeight() });
			}

			transform->SetFlip(info.flip);
		});

	m_loader->AddObjectLayerHandler("ExplorationTrigger", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;
			float w = obj->FloatAttribute("width") * scale.x;
			float h = obj->FloatAttribute("height") * scale.y;

			Entity* triggerZone = scene->CreateEntityNow();
			TransformComponent* transform = triggerZone->GetComponent<TransformComponent>();
			triggerZone->GetComponent<RenderLayerComponent>()->SetLayer(1);

			transform->SetPosition({ x + w * 0.5f, y + h * 0.5f });

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "ERROR");
			std::string direction_str = TiledUtils::GetProperty<std::string>(obj, "Direction", "Any");
			TriggerDirection direction;

			if (direction_str == "LeftToRight")
				direction = TriggerDirection::LeftToRight;
			else if (direction_str == "RightToLeft")
				direction = TriggerDirection::RightToLeft;
			else if (direction_str == "TopToBottom")
				direction = TriggerDirection::TopToBottom;
			else if (direction_str == "BottomToTop")
				direction = TriggerDirection::BottomToTop;
			else if (direction_str == "Any")
				direction = TriggerDirection::Any;

			triggerZone->AddComponent<ExplorationTriggerLogic>("Player", direction);

			triggerZone->AddComponent<BoxCollider>(w, h)->SetTrigger(true);
			triggerZone->GetComponent<TagComponent>()->AddTag(tag);
		});

	m_loader->AddObjectLayerHandler("CameraPOI", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;

			Entity* camPOI = scene->CreateEntityNow();
			TransformComponent* transform = camPOI->GetComponent<TransformComponent>();
			camPOI->GetComponent<RenderLayerComponent>()->SetLayer(1);

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "ERROR");

			transform->SetPosition({ x, y });

			camPOI->GetComponent<TagComponent>()->AddTag(tag);

			GameManager::Get()->CreateStatsWindow("CameraPOI")->Inspect(camPOI);
		});

	m_loader->AddObjectLayerHandler("Door", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;
			float w = obj->FloatAttribute("width") * scale.x;
			float h = obj->FloatAttribute("height") * scale.y;

			Entity* door = scene->CreateEntityNow();
			door->GetComponent<TransformComponent>()->SetPosition(x + w * 0.5, y + h * 0.5);
			door->GetComponent<RenderLayerComponent>()->SetLayer(1);

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "ERROR");
			std::string monde = TiledUtils::GetProperty<std::string>(obj, "Monde", "");

			TagComponent* tc = door->GetComponent<TagComponent>();
			tc->AddTag(tag);

			if(monde != "")
				tc->AddTag(monde);

			BoxCollider* bc = door->AddComponent<BoxCollider>(w, h);
			bc->SetTrigger(true);
			bc->SetVisible(false);

			door->AddComponent<DoorLogic>();
		});

	m_loader->AddObjectLayerHandler("PlayerSpawn", [](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;

			Entity* playerSpawn = scene->CreateEntityNow();
			playerSpawn->GetComponent<TransformComponent>()->SetPosition(x, y);

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "ERROR");
			playerSpawn->GetComponent<TagComponent>()->AddTag(tag);
		});

	m_loader->AddObjectLayerHandler("Collectible", [this](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale) {

		PropInfo info = GetPropInfo(obj, scale);
		CreateCollectible(info.source, info.x, info.y, info.rotation, { info.w, info.h });
		AmbiantShader::Get()->AddLight({ info.x + 50.f, info.y-50.f }, 100.f, 3.f, { 1.f, 1.f, 1.f });
		});
	
	m_loader->AddObjectLayerHandler("Npc", [this](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale) {

		float x = obj->FloatAttribute("x") * scale.x;
		float y = obj->FloatAttribute("y") * scale.y;

		std::string textureName = TiledUtils::GetProperty<std::string>(obj, "Texture Name", "object");
		std::string texturePath = "../../../Assets/" + textureName + ".png";

		std::string dialogueText = TiledUtils::GetProperty<std::string>(obj, "Dialogue Text", "ERROR !");

		float scaleX = TiledUtils::GetProperty<float>(obj, "ScaleX", 0.3f);
		float scaleY = TiledUtils::GetProperty<float>(obj, "ScaleY", 0.3f);

		this->CreateNPC(texturePath, x, y, dialogueText, {scaleX, scaleY});
		});

	m_loader->AddObjectLayerHandler("BatSpawn", [this](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;

			Entity* BatSpawn = scene->CreateEntityNow();
			BatSpawn->GetComponent<TransformComponent>()->SetPosition(x, y);

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "ERROR");
			BatSpawn->GetComponent<TagComponent>()->AddTag(tag);
			Entity* Bat = CreateBat(x, y);
			
		});
	
	m_loader->AddObjectLayerHandler("RatSpawn", [this](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;

			Entity* RatSpawn = scene->CreateEntityNow();
			RatSpawn->GetComponent<TransformComponent>()->SetPosition(x, y);

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "ERROR");
			RatSpawn->GetComponent<TagComponent>()->AddTag(tag);
			Entity* Rat = CreateRat(x, y);
		});

	m_loader->AddObjectLayerHandler("BossSpawn", [this](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
	{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;

			Entity* BossSpawn = scene->CreateEntityNow();
			BossSpawn->GetComponent<TransformComponent>()->SetPosition(x, y);

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "ERROR");
			BossSpawn->GetComponent<TagComponent>()->AddTag(tag);
	});
	
	m_loader->AddObjectLayerHandler("RatSpawn", [this](tinyxml2::XMLElement* obj, Scene* scene, sf::Vector2f scale)
		{
			float x = obj->FloatAttribute("x") * scale.x;
			float y = obj->FloatAttribute("y") * scale.y;

			Entity* RatSpawn = scene->CreateEntityNow();
			RatSpawn->GetComponent<TransformComponent>()->SetPosition(x, y);

			std::string tag = TiledUtils::GetProperty<std::string>(obj, "Tag", "ERROR");
			RatSpawn->GetComponent<TagComponent>()->AddTag(tag);
			Entity* Rat = CreateRat(x, y);
		});

	m_loader->Load("../../../Assets/mapTest.tmx", this, "../../../Assets/placeholder.png");

	atlas->Build();

	std::unordered_map<int, std::unordered_map<int, PropRenderer*>> renderers;

	auto buildLayer = [&](std::vector<PropInfo>& props, int layer)
		{
			for (PropInfo& p : props)
			{
				AtlasRect ar = atlas->GetRect(p.source);

				if (!renderers[layer].count(ar.atlasIndex))
				{
					Entity* e = CreateEntityNow();
					e->GetComponent<RenderLayerComponent>()->SetLayer(layer);

					renderers[layer][ar.atlasIndex] =
						e->AddComponent<PropRenderer>(atlas->GetTexture(ar.atlasIndex));
				}

				renderers[layer][ar.atlasIndex]->AddProp({ p.x, p.y }, ar.rect, { p.w / ar.rect.width, p.h / ar.rect.height }, p.rotation, p.flip);
			}
		};

	buildLayer(propsBack, -1);
	buildLayer(propsFront, 1);
	buildLayer(propsFrontPlayer, 3);

	Entity* musicEntity = CreateEntityNow();
	Music* music = musicEntity->AddComponent<Music>();
	music->Load("../../../Assets/BossMusic.ogg", false);
	music->SetVolume(100);
	music->SetSpacialize(false);
	musicEntity->GetComponent<TagComponent>()->AddTag("rhythmMusic");

	

	Entity* helper = CreateEntityNow();
	helper->AddComponent<Sprite>("../../../Assets/object.png");
	helper->GetComponent<Sprite>()->SetVisible(false);
	helper->GetComponent<TransformComponent>()->SetScale({ 0.3, 0.3 });
	helper->GetComponent<RenderLayerComponent>()->SetLayer(100);
	helper->GetComponent<TagComponent>()->AddTag("Help");

	Entity* AmbianteMusic = CreateEntityNow();
	AmbianteMusic->AddComponent<Music>()->Load("../../../Assets/AmbiantMusic/Jazz.wav",true);
	AmbianteMusic->AddComponent<Music>()->Load("../../../Assets/AmbiantMusic/Rock.wav", true);
	for (int i = 0; i < 2; i++) {
		AmbianteMusic->GetAllComponents<Music>()[i]->SetSpacialize(false);
		AmbianteMusic->GetAllComponents<Music>()[i]->SetVolume(20.f);
	}
	AmbianteMusic->GetComponent<TagComponent>()->AddTag("AmbiantMusic");

	Entity* dialogueBg = CreateEntityNow();
	dialogueBg->AddComponent<Sprite>("../../../Assets/speechBubble.png");
	dialogueBg->GetComponent<Sprite>()->SetVisible(false);
	dialogueBg->GetComponent<RenderLayerComponent>()->SetLayer(100);
	dialogueBg->GetComponent<TagComponent>()->AddTag("Text");

	Entity* player = CreatePlayer();
	GameManager::Get()->CreateStatsWindow("PlayerStats")->Inspect(player);

	Entity* HealthBar = CreateHealth(150, 50);

	Entity* Collapse = CreateEntityNow();
	Collapse->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/earthquake.wav", false);
	Collapse->GetComponent<SoundEffect>()->SetSpacialize(false);
	Collapse->GetComponent<TagComponent>()->AddTag("Collapse");



	Entity* MusicChangeUi = CreateMusicChangeUi(1155, 71);

	AmbiantShader::Get()->AddPlayer(player, 200, 2.0, {255.f / 255, 255.f / 255.f, 255.f / 255.f });

	Entity* camEnt = CreateEntityNow();
	camEnt->GetComponent<RenderLayerComponent>()->SetLayer(2);
	camEnt->GetComponent<TagComponent>()->AddTag("Camera");
	Camera* cam = camEnt->AddComponent<Camera>();
	cam->SetLink(player);
	cam->SetMainCamera();

	GameManager::Get()->GetALphaTexture()->setFillColor(sf::Color::Black);
	cam->FadeIn(5);

	cam->SetSmooth(true);
	cam->SetOffset({ 0, -100 });

	StatsWindow* camWindow = GameManager::Get()->CreateStatsWindow("Camera");
	camWindow->Inspect(camEnt);

	Entity* barman = CreateEntityNow();
	TransformComponent* tc = barman->GetComponent<TransformComponent>();
	tc->SetPosition(2145, 5107);
	tc->SetScale({ 0.3, 0.3 });
	barman->AddComponent<Sprite>("../../../Assets/barman.png")->SetSrcRect(0, 0, 306, 326);
	barman->AddComponent<Animation>("../../../Assets/barman.json")->SetAnimation("default");
	barman->GetComponent<RenderLayerComponent>()->SetLayer(0);

	//Entity* ET = FindEntityWithTag("tag");
	//ET->GetComponent<ExplorationTriggerLogic>()->SetOnExit([](CollisionSide side) {camera.TransitionTo("campoi")});

	{
		Entity* door = FindEntityWithTag("DoorToPunkBar");
		door->GetComponent<DoorLogic>()->SetOnOpen([this](TransformComponent* playerTransform) {
			if (FindEntityWithTag("Player")->GetComponent<Inventory>()->HasAllItems()) {
				playerTransform->SetPosition(FindEntityWithTag("PunkBar")->GetComponent<TransformComponent>()->GetPosition());
				FindEntityWithTag("Camera")->GetComponent<Camera>()->FadeIn(2.f);
			}
			});
		door->GetComponent<DoorLogic>()->SetWaitOnEnter(2.f);

		door->GetComponent<DoorLogic>()->SetOnClick([this]() {
			if (FindEntityWithTag("Player")->GetComponent<Inventory>()->HasAllItems()) {
				FindEntityWithTag("Camera")->GetComponent<Camera>()->FadeOut(2.f);
			}
			});
	}
	
	{
		Entity* door = FindEntityWithTag("DoorExitStringItem");
		door->GetComponent<DoorLogic>()->SetOnOpen([this](TransformComponent* playerTransform) {
			playerTransform->SetPosition(FindEntityWithTag("TpEgout")->GetComponent<TransformComponent>()->GetPosition());
			FindEntityWithTag("Camera")->GetComponent<Camera>()->FadeIn(2.f);
			});
		door->GetComponent<DoorLogic>()->SetWaitOnEnter(2.f);

		door->GetComponent<DoorLogic>()->SetOnClick([this]() {
			FindEntityWithTag("Camera")->GetComponent<Camera>()->FadeOut(2.f);
			});
	}

	{

		m_rhythmSpawner = new RhythmSpawner(this, 6.f);
		m_rhythmSpawner->LoadMap("../../../Assets/test2.json");




		Load("../../../Assets/saveTest.json");
		ShaderManager::Get()->SetCurrentMainShader("../../../src/Shaders/AmbiantShader.frag");


		for (Entity* entity : FindAllEntitiesWithTag("Jazz")) {
			if (Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(false);
			if (Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(false);
		}
		for (Entity* entity : FindAllEntitiesWithTag("Rock")) {
			if (Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(false);
			if (Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(false);
		}
		for (Entity* entity : FindAllEntitiesWithTag("postCollapse")) {
			if (Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(false);
			if (Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(false);
		}

		Sprite* collapseSprite = FindEntityWithTag("ecroulement")->GetComponent<Sprite>();
		collapseSprite->SetSrcRect(0, 0, 2680, 1000);

	}

	{

		FindEntityWithTag("MusicChange")->GetComponent<Sprite>()->SetSrcRect(0, 0, 380, 206);
		Entity* trigger1 = FindEntityWithTag("EntreeEgout1");
		trigger1->GetComponent<ExplorationTriggerLogic>()->SetOnExit([this](CollisionSide side) {
			ShaderManager::Get()->SetParameter<float>("../../../src/Shaders/AmbiantShader.frag", "darkness", 0.9);
			AmbiantShader::Get()->AddPlayer(FindEntityWithTag("Player"), 300.f, 2.0, { 255.f / 255, 255.f / 255.f, 255.f / 255.f });

			//Entity* entity = FindEntityWithTag("POIEgout");
			//FindEntityWithTag("Camera")->GetComponent<Camera>()->TransitionTo({ 4588.f, 1854.f, 2240.f, 1024.f }, 500, 500);
			});
		Entity* trigger2 = FindEntityWithTag("SortieEgout1");
		trigger2->GetComponent<ExplorationTriggerLogic>()->SetOnExit([this](CollisionSide side) {
			ShaderManager::Get()->SetParameter<float>("../../../src/Shaders/AmbiantShader.frag", "darkness", 0.6);
			AmbiantShader::Get()->AddPlayer(FindEntityWithTag("Player"), 200.f, 2.0, { 255.f / 255, 255.f / 255.f, 255.f / 255.f });

			//FindEntityWithTag("Camera")->GetComponent<Camera>()->TransitionTo(FindEntityWithTag("Player"), {1280, 720}, 500, 500);
			});
		Entity* trigger3 = FindEntityWithTag("EntreeEgout2");
		trigger3->GetComponent<ExplorationTriggerLogic>()->SetOnExit([this](CollisionSide side) {
			ShaderManager::Get()->SetParameter<float>("../../../src/Shaders/AmbiantShader.frag", "darkness", 0.9);
			AmbiantShader::Get()->AddPlayer(FindEntityWithTag("Player"), 300.f, 2.0, { 255.f / 255, 255.f / 255.f, 255.f / 255.f });

			//Entity* entity = FindEntityWithTag("POIEgout");
			//FindEntityWithTag("Camera")->GetComponent<Camera>()->TransitionTo({4588.f, 1854.f, 2240.f, 1024.f}, 500, 500);
			});
		Entity* trigger4 = FindEntityWithTag("SortieEgout2");
		trigger4->GetComponent<ExplorationTriggerLogic>()->SetOnExit([this](CollisionSide side) {
			ShaderManager::Get()->SetParameter<float>("../../../src/Shaders/AmbiantShader.frag", "darkness", 0.6);
			AmbiantShader::Get()->AddPlayer(FindEntityWithTag("Player"), 200.f, 2.0, { 255.f / 255, 255.f / 255.f, 255.f / 255.f });

			//FindEntityWithTag("Camera")->GetComponent<Camera>()->TransitionTo(FindEntityWithTag("Player"), {1280, 720}, 500, 500);
			});
		Entity* trigger5 = FindEntityWithTag("triggerCollapse");
		trigger5->GetComponent<ExplorationTriggerLogic>()->SetOnExit([this](CollisionSide side) {

			for (Entity* entity : FindAllEntitiesWithTag("temp")) {
				entity->Destroy();
			}

			int musicType = FindEntityWithTag("Player")->GetComponent<MusicChange>()->GetMusicType();
			std::string currentWorld;

			switch (musicType)
			{
			case 0:
				currentWorld = "DEFAULT";
				break;
			case 1:
				currentWorld = "Jazz";
				break;
			case 2:
				currentWorld = "Rock";
				break;
			}

			FindEntityWithTag("Camera")->GetComponent<Camera>()->Shake(6.f, 10.f);
			FindEntityWithTag("Collapse")->GetComponent<SoundEffect>()->PlaySoundEffect();

			for (Entity* entity : FindAllEntitiesWithTag("postCollapse")) {
				TagComponent* tc = entity->GetComponent<TagComponent>();
				if (tc->Is(currentWorld) || (tc->Is("Jazz") == false && tc->Is("Rock") == false))
				{
					if (Sprite* sprite = entity->GetComponent<Sprite>()) sprite->SetVisible(true);
					if (Collider* collider = entity->GetComponent<Collider>()) collider->SetActive(true);
				}
			}
			FindEntityWithTag("triggerCollapse")->Destroy();
			});


	}

	{
		Entity* door2 = FindEntityWithTag("DoorToBossRoom");
		door2->GetComponent<DoorLogic>()->SetOnOpen([this](TransformComponent* playerTransform) {

			FindEntityWithTag("Player")->GetComponent<Rigidbody2D>()->SetKinematic(true);

			Entity* camera2 = FindEntityWithTag("Camera");
			camera2->GetComponent<Camera>()->FadeIn(1.f);

			playerTransform->SetPosition(FindEntityWithTag("BossCameraPOI")->GetComponent<TransformComponent>()->GetPosition());
			Entity* entity = FindEntityWithTag("Player");
			entity->GetComponent<Sprite>()->SetVisible(false);
			ShaderManager::Get()->SetParameter<float>("../../../src/Shaders/AmbiantShader.frag", "darkness", 0.2);

			sf::Vector2f pos1 = FindEntityWithTag("RythmeSpawn")->GetComponent<TransformComponent>()->GetPosition();
			sf::Vector2f pos2 = FindEntityWithTag("HypeBarSpawn")->GetComponent<TransformComponent>()->GetPosition();

			FindEntityWithTag("healthBar")->GetComponent<Sprite>()->SetVisible(false);
			FindEntityWithTag("MusicChange")->GetComponent<Sprite>()->SetVisible(false);

			for (Entity* e : FindAllEntitiesWithTag("Inventory")) {
				e->GetComponent<Sprite>()->SetVisible(false);
			}

			CreateRythmEntities(pos1);
			Entity* hypeBar = CreateHypeBar(pos2);

			FindEntityWithTag("Player")->GetComponent<PlayerMovement>()->SetInBossRoom(true);

			for (auto* s : FindEntityWithTag("AmbiantMusic")->GetAllComponents<Music>()) {
				s->Stop();
			}

			m_rhythmSpawner->Stop();
			m_rhythmSpawner->Play();

			});
		door2->GetComponent<DoorLogic>()->SetWaitOnEnter(2.5);


		door2->GetComponent<DoorLogic>()->SetOnClick([this]() {
			Entity* camera2 = FindEntityWithTag("Camera");
			camera2->GetComponent<Camera>()->FadeOut(2.f);


			});
	}


	{
		Entity* ButtonExit = CreateEntity();
		Button* button = ButtonExit->AddComponent<Button>();


		button->SetOnClick([this]() { ReturnMenu(); });
		button->SetSize({ 442, 145 });
		button->SetActivate(false);
		TransformComponent* transform = ButtonExit->GetComponent<TransformComponent>();
		transform->SetPosition({ 3750, 5350 });
		transform->SetScale({ 0.7, 0.7 });

		Sprite* sprite = ButtonExit->AddComponent<Sprite>("../../../Assets/ux/quit_death_screen.png");
		sprite->SetOffset({ 221 * transform->GetScale().x, 72 * transform->GetScale().x });
		//sprite->SetUI(true);
		sprite->SetVisible(false);



		ButtonExit->GetComponent<TagComponent>()->AddTag("ButtonExit");
	}
}

void StartScene::OnLoading(){
	sf::RenderWindow* window = GameManager::Get()->GetWindow();
	float progression = 0.f;
	while (progression != 100) {
		progression = atlas->GetBuildProgress();
		window->clear();

		sf::Sprite sprite;
		sprite.setTexture(*AssetsManager::Get()->Load("../../../Assets/logo.png"));
		sprite.setScale({ 0.4, 0.4 });
		sprite.setPosition({ 250, 50 });
		window->draw(sprite);


		sf::RectangleShape loading(sf::Vector2f(window->getSize()));
		loading.setFillColor(sf::Color(238, 222, 197, 255));
		loading.setPosition({360, 550});
		loading.setSize({560 * (progression/100), 20});

		window->draw(loading);


		window->display();
	}
}

void StartScene::OnUpdate() {

	if (InputManager::Get()->IsDown(sf::Keyboard::S))
	{
		Save("../../../Assets/saveTest.json");
	}

	if (InputManager::Get()->IsDown(sf::Keyboard::L))
	{
		Load("../../../Assets/saveTest.json");
	}

	if ((InputManager::Get()->IsDown(KeyBinds::Get()->GetControllerBind("Pause")) || InputManager::Get()->IsDown(KeyBinds::Get()->GetKeyboardBind("Pause"))) && FindEntityWithTag("Player")->GetComponent<PlayerMovement>()->IsInBossRoom() == false) {
		SceneManager::GetInstance().ChangeScene("Pause");
		SceneManager::GetInstance().FindSceneById("Pause")->SetInitalize(true);
	}
	m_rhythmSpawner->Update(GetDeltaTime());


	AmbiantShader::Get()->Update();
	ShaderManager::Get()->SetVisible(true);

	
}

Entity* StartScene::CreateMusicChangeUi(int x, int y)
{
	Entity* MusicChange = CreateEntityNow();
	MusicChange->AddComponent<Sprite>("../../../Assets/MusicChange.png");
	MusicChange->GetComponent<RenderLayerComponent>()->SetLayer(100);
	MusicChange->GetComponent<TransformComponent>()->SetPosition(x, y);
	MusicChange->GetComponent<TransformComponent>()->SetScale(0.5f, 0.5f);
	MusicChange->GetComponent<TagComponent>()->AddTag("MusicChange");
	MusicChange->GetComponent<Sprite>()->SetUI(true);

	return MusicChange;
}

Entity* StartScene::CreateHealth(int x, int y)
{
	Entity* HealthBar = CreateEntityNow();
	HealthBar->AddComponent<Sprite>("../../../Assets/Hpbar.png");
	HealthBar->GetComponent<RenderLayerComponent>()->SetLayer(100);
	HealthBar->GetComponent<TransformComponent>()->SetPosition(x, y);
	HealthBar->GetComponent<TransformComponent>()->SetScale(0.2f, 0.2f);
	HealthBar->GetComponent<TagComponent>()->AddTag("healthBar");
	HealthBar->GetComponent<Sprite>()->SetUI(true);

	return HealthBar;
}

Entity* StartScene::CreateRat(int x, int y)
{
	Entity* Rat = CreateEntityNow();

	TransformComponent* transform = Rat->GetComponent<TransformComponent>();

	transform->SetScale({ 0.43,  0.43 });

	Rat->AddComponent<BoxCollider>(100.f, 48.f);
	Rat->GetComponent<TagComponent>()->AddTag("DamageZone");
	Rat->GetComponent<TagComponent>()->AddTag("Rat");
	Rat->GetComponent<TransformComponent>()->SetPosition(x + 100, y);
	Rat->AddComponent<Rigidbody2D>(1.f, true, 0.f)->SetSafeMargin(0.f);
	Rat->AddComponent<RatMovement>();
	SoundEffect* se = Rat->AddComponent<SoundEffect>();
	se->Load("../../../Assets/SoundDesign/mob/rat/ratMieux/RatAttack.wav", false);
	se->SetCooldown(20.f);
	se->SetSpacialize(true);
	se->SetMinDistance(100.f);
	se->SetVolume(100.f);
	Rat->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/mob/rat/ratMieux/RatDeath.wav", false);
	Rat->GetAllComponents<SoundEffect>()[1]->SetCooldown(20.f);
	Rat->GetAllComponents<SoundEffect>()[1]->SetSpacialize(false);
	Rat->AddComponent<Sprite>("../../../Assets/rat.png");
	Rat->AddComponent<Animation>("../../../Assets/rat_walk.json")->SetAnimation("walk");

	GameManager::Get()->CreateStatsWindow("Rat")->Inspect(Rat);
	return Rat;
}

Entity* StartScene::CreatePlayer()
{
	Entity* player = CreateEntityNow();

	player->SetSavable(true);
	player->GetComponent<RenderLayerComponent>()->SetLayer(2);
	player->AddComponent<Listener>();

	TransformComponent* transform = player->GetComponent<TransformComponent>();
	transform->SetPosition(FindEntityWithTag("First")->GetComponent<TransformComponent>()->GetPosition());
	transform->SetScale({ 0.43,  0.43 });

	player->AddComponent<BoxCollider>(38.f, 175.f);
	player->AddComponent<Rigidbody2D>(1.f, true, 0.f)->SetSafeMargin(0.8f);

	player->AddComponent<Sprite>("../../../Assets/playerSheet.png")->SetSrcRect(0, 0, 349, 409);
	player->GetComponent<TagComponent>()->AddTag("Player");
	player->AddComponent<PlayerMovement>();
	player->AddComponent<PlayerWalljump>();
	player->AddComponent<PlayerHealth>();
	player->AddComponent<MusicChange>();
	player->AddComponent<Inventory>();
	player->AddComponent<PlayerInteract>();



	Inventory* inv = player->GetComponent<Inventory>();

	inv->AddSlot(CreateSlot(0));
	inv->AddSlot(CreateSlot(1));
	inv->AddSlot(CreateSlot(2));

	inv->SetRewardTexture("../../../Assets/bass.png");

	player->AddComponent<PlayerAnimationController>();
	player->AddComponent<Animation>("../../../Assets/playerAnim.json")->SetAnimation("idle");

	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/marche/0.Wav", false);
	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/marche/0.Wav", false);
	player->GetAllComponents<SoundEffect>()[1]->SetCooldown(0.4f);
	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/playersounds/Vomissement.wav", false);
	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/playersounds/Wallglide.wav", false);
	player->GetAllComponents<SoundEffect>()[3]->SetCooldown(5.f);
	player->GetAllComponents<SoundEffect>()[3]->SetVolume(100.f);
	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/playersounds/hurt/Hurt.Wav", false);
	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/playersounds/jump/Jump.Wav", false);
	player->GetAllComponents<SoundEffect>()[5]->SetCooldown(0.01f);
	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/playersounds/jump/JumpLand.Wav", false);
	player->GetAllComponents<SoundEffect>()[6]->SetCooldown(0.25f);
	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/playersounds/swallow/swallow.Wav", false);
	player->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/Menu_et_item/Item2A.wav", false);

	for (int i = 0; i < 9; i++) {
		player->GetAllComponents<SoundEffect>()[i]->SetSpacialize(false);
	}


	player->GetComponent<SoundEffect>()->SetSpacialize(false);

	return player;
}

Entity* StartScene::CreateSlot(int index)
{
	Entity* slot = CreateEntityNow();

	slot->AddComponent<Sprite>("../../../Assets/marco.png")->SetUI(true);
	slot->GetComponent<Sprite>()->SetVisible(true);
	slot->GetComponent<RenderLayerComponent>()->SetLayer(80);
	slot->GetComponent<TransformComponent>()->SetPosition(568 + 72 * index, 600);
	slot->GetComponent<TagComponent>()->AddTag("Inventory");
	slot->GetComponent<TagComponent>()->AddTag("Slot" + std::to_string(index + 1));

	return slot;
}

Entity* StartScene::CreateBat(int x, int y)
{
	Entity* Bat = CreateEntityNow();

	TransformComponent* transform = Bat->GetComponent<TransformComponent>();

	transform->SetScale({ 0.60,  0.60 });

	
	Bat->AddComponent<BoxCollider>(74.f, 67.f);
	Bat->GetComponent<TagComponent>()->AddTag("DamageZone");
	Bat->GetComponent<TagComponent>()->AddTag("Bat");
	Bat->GetComponent<TransformComponent>()->SetPosition(x, y);
	Bat->GetComponent<BoxCollider>()->SetTrigger(true);
	Bat->AddComponent<BatMovement>();
	Bat->AddComponent<Sprite>("../../../Assets/BAT.png");
	Bat->AddComponent<SoundEffect>()->Load("../../../Assets/SoundDesign/mob/bat/bat01.ogg", false);
	Bat->GetComponent<SoundEffect>()->SetCooldown(20.f);
	Bat->GetComponent<SoundEffect>()->SetSpacialize(true);
	Bat->GetComponent<SoundEffect>()->SetMinDistance(100.f);
	Bat->GetComponent<SoundEffect>()->SetVolume(100.f);

	
	Bat->AddComponent<Animation>("../../../Assets/chauve_souris.json")->SetAnimation("flying");
	return Bat;
}

Entity* StartScene::CreateCollectible(const std::string& path, float x, float y, float rotation, sf::Vector2f size)
{
	Entity* item = CreateEntityNow();
	TransformComponent* transform = item->GetComponent<TransformComponent>();
	item->GetComponent<RenderLayerComponent>()->SetLayer(1);
	item->AddComponent<BoxCollider>(264.f, 264.f);
	item->GetComponent<BoxCollider>()->SetTrigger(true);
	Sprite* sprite = item->AddComponent<Sprite>(path);
	item->AddComponent<ItemData>()->texturePath = path;
	transform->SetRotation(rotation);
	transform->SetPosition(x + size.x * 0.5f, y - size.y * 0.5f);
	transform->SetScale(size.x / sprite->GetWidth(), size.y / sprite->GetHeight());
	item->GetComponent<TagComponent>()->AddTag("Collectible");

	return item;
}

Entity* StartScene::CreateConsommable(const std::string& path, const std::string& tag, float x, float y, sf::Vector2f scale)
{
	Entity* item = CreateEntityNow();
	TransformComponent* transform = item->GetComponent<TransformComponent>();

	item->AddComponent<BoxCollider>(264.f, 264.f);
	item->GetComponent<BoxCollider>()->SetTrigger(true);
	item->AddComponent<Sprite>(path);
	item->AddComponent<ItemData>()->texturePath = path;
	transform->SetPosition(x, y);
	transform->SetScale(scale);
	TagComponent* tc = item->GetComponent<TagComponent>();
	tc->AddTag("Consommable");

	if (tag == "PeeBottle")
	{
		tc->AddTag("Damage");
		FindEntityWithTag("Player")->GetAllComponents<SoundEffect>()[5]->PlaySoundEffect();
	}
	else if (tag == "BeerBottle" || tag == "WhiskyBottle")
	{
		tc->AddTag("Heal");
		FindEntityWithTag("Player")->GetAllComponents<SoundEffect>()[7]->PlaySoundEffect();
	}

	return item;
}

Entity* StartScene::CreateNPC(std::string path, float x, float y, std::string dialogue, sf::Vector2f scale)
{
	Entity* m_npc = CreateEntityNow();
	TransformComponent* npc = m_npc->GetComponent<TransformComponent>();

	m_npc->AddComponent<BoxCollider>(264.f, 264.f)->SetTrigger(true);
	m_npc->AddComponent<Sprite>(path);
	m_npc->GetComponent<TagComponent>()->AddTag("Interactable");
	m_npc->AddComponent<TextComponent>("../../../Assets/newshound-regular.otf", 32);
	npc->SetScale(scale);
	npc->SetPosition(x, y);

	ItemData* data = m_npc->AddComponent<ItemData>();
	data->dialogue = dialogue;

	return m_npc;
}

Entity* StartScene::CreateRythmEntities(sf::Vector2f pos)
{
	Entity* rythmZone = CreateEntityNow();
	TransformComponent* transform = rythmZone->GetComponent<TransformComponent>();

	transform->SetPosition(pos);
	transform->SetScale(0.86, 0.86);

	rythmZone->AddComponent<BoxCollider>(75.f, 300.f)->SetTrigger(true);
	//rythmZone->AddComponent<Sprite>("../../../src/Riot_Girl.png");
	rythmZone->GetComponent<TagComponent>()->AddTag("RythmZone");
	rythmZone->AddComponent<CheckZoneLogic>();

	Sprite* sprite = rythmZone->AddComponent<Sprite>("../../../Assets/ux/guitar_bar.png");
	sprite->SetOffset({ 550, 0 });
	//sprite->SetUI(true);
	
	Sprite* sprite2 = rythmZone->AddComponent<Sprite>("../../../Assets/ux/capo.png");
	rythmZone->GetComponent<RenderLayerComponent>()->SetLayer(100);
	
	//sprite2->SetUI(true);



	Entity* stopper = CreateEntityNow();
	stopper->GetComponent<TransformComponent>()->SetPosition({ pos.x-200, pos.y });
	stopper->AddComponent<BoxCollider>(75.f, 300.f);

	stopper->GetComponent<TagComponent>()->AddTag("Stopper");

	RythmStats = GameManager::Get()->CreateStatsWindow("Rythm Stats");
	RythmStats->Inspect(rythmZone);

	return rythmZone;
}

Entity* StartScene::CreateHypeBar(sf::Vector2f pos)
{
	Entity* hypeBar = CreateEntityNow();
	HypeBar* hype = hypeBar->AddComponent<HypeBar>(100, 25);
	hypeBar->GetComponent<RenderLayerComponent>()->SetLayer(100);

	TransformComponent* transform = hypeBar->GetComponent<TransformComponent>();
	transform->SetPosition(pos);
	transform->SetScale(0.5, 0.4);



	hypeBar->GetComponent<RenderLayerComponent>()->SetLayer(100);

	hypeBar->GetComponent<TagComponent>()->AddTag("hypeBar");

	Sprite* sprite = hypeBar->AddComponent<Sprite>("../../../Assets/ux/hypebar_us.png");
	sprite->SetOffset({ 0.f, 0.f });

	Sprite* sprite2 = hypeBar->AddComponent<Sprite>("../../../Assets/ux/hype_bar_indicator.png");
	sprite2->SetOffset({ 0.f, 0 });
	

	hype->SetHypeIndicator(0.5);

	GameManager::Get()->CreateStatsWindow("HypeBar")->Inspect(hypeBar);

	SoundEffect* sound = hypeBar->AddComponent<SoundEffect>();
	sound->SetSpacialize(false);
	sound->SetCooldown(6.f);

	sound->Load("../../../Assets/SoundDesign/Foule.wav", true);
	sound->SetSpacialize(false);

	return hypeBar;
}
