#include "Scene.h"
#include "Entity.h"
#include "GameManager.h"
#include "CollisionSystem.h"
#include "TransformComponent.h"
#include "TagComponent.h"
#include "TilemapLoader.h"
#include "RenderLayerComponent.h"
#include "Savable.h"
#include <fstream>

Scene::Scene()
{
	mpGameManager = nullptr;
	mCollisionSystem = new CollisionSystem;
	m_loader = new TilemapLoader;
}

Scene::~Scene()
{
	delete mCollisionSystem;
	delete m_loader;

	for (Entity* ent : mEntities)
	{
		delete ent;
	}
}

int Scene::GetWindowWidth() const
{
	return mpGameManager->mWindowWidth;
}

int Scene::GetWindowHeight() const
{
	return mpGameManager->mWindowHeight;
}

void Scene::DestroyEntityNow(Entity* _ent)
{
	auto it = std::find(mEntities.begin(), mEntities.end(), _ent);

	if (it != mEntities.end())
	{
		mCollisionSystem->Clear(*it);
		delete* it;
		mEntities.erase(it);
	}
}

void Scene::DestroyAllEntities()
{
	for (auto it = mEntities.begin(); it != mEntities.end(); )
	{
		mCollisionSystem->Clear(*it);
		delete* it;
		it = mEntities.erase(it); 
	}
}

void Scene::DestroyEntityWithTag(const std::string& _tag)
{
	FindEntityWithTag(_tag)->Destroy();
}

void Scene::DestroyAllEntitiesWithTag(const std::string& _tag)
{
	for (Entity* ent : FindAllEntitiesWithTag(_tag))
		ent->Destroy();
}

Entity* Scene::FindEntityWithTag(const std::string& _tag)
{
 	for (Entity* ent : mEntities)
	{
		if (ent->GetComponent<TagComponent>()->Is(_tag))
			return ent;
	}

	return nullptr;
}

std::vector<Entity*> Scene::FindAllEntitiesWithTag(const std::string& _tag)
{
	std::vector<Entity*> result;

	for (Entity* ent : mEntities)
	{
		if (ent->GetComponent<TagComponent>()->Is(_tag))
			result.push_back(ent);
	}

	return result;
}

Entity* Scene::FindEntityByID(uint64_t id)
{
	for (Entity* e : mEntities)
	{
		if (e->GetID() == id)
			return e;
	}

	return nullptr;
}

void Scene::Save(const std::string & path)
{
	json root;

	for (Entity* entity : mEntities)
	{
		if (entity->ShouldSave() == false)
			continue;

		json entityJson;

		std::vector<Savable*> savables = entity->GetAllComponents<Savable>();

		for (Savable* s : savables)
		{
			entityJson["components"][s->GetSaveName()] = s->Save();
		}

		entityJson["id"] = entity->GetID();

		root["entities"].push_back(entityJson);
	}

	std::ofstream file(path);
	file << root.dump(4);
}

void Scene::Load(const std::string& path)
{
	std::ifstream file(path);

	if (file.is_open() == false)
		return;

	json root;
	file >> root;

	for (auto& entityData : root["entities"])
	{
		uint64_t id = entityData["id"];

		Entity* entity = FindEntityByID(id);

		if (entity == nullptr)
			continue;

		if (entity->ShouldSave() == false)
			continue;

		std::vector<Savable*> savables = entity->GetAllComponents<Savable>();

		for (Savable* s : savables)
		{
			auto comps = entityData["components"];

			if (comps.contains(s->GetSaveName()))
			{
				s->Load(comps[s->GetSaveName()]);
			}
		}
	}
}

Entity* Scene::CreateEntity()
{
	Entity* ent = new Entity;
	ent->AddComponent<TransformComponent>();
	ent->AddComponent<TagComponent>();
	ent->AddComponent<RenderLayerComponent>()->SetLayer(0);
	mEntitiesToAdd.push_back(ent);
	return ent;
}

Entity* Scene::CreateEntityNow()
{
	Entity* ent = new Entity;
	ent->AddComponent<TransformComponent>();
	ent->AddComponent<TagComponent>();
	ent->AddComponent<RenderLayerComponent>()->SetLayer(0);
	mEntities.push_back(ent);
	return ent;
}

float Scene::GetDeltaTime() const
{
	return mpGameManager->mDeltaTime;
}

float Scene::GetFixedDeltaTime() const
{
	return mpGameManager->GetFixedDeltaTime();
}

void Scene::Initialize()
{
	OnInitialize();
}

void Scene::Update()
{
	float& accDt = mpGameManager->GetAccumulateDeltaTime();
	accDt += GetDeltaTime();
	while (accDt >= GetFixedDeltaTime())
	{
		FixedUpdate();
		accDt -= GetFixedDeltaTime();
	}

	//Update
	for (auto it = mEntities.begin(); it != mEntities.end(); )
	{
		Entity* entity = *it;

		entity->Update();

		if (entity->ToDestroy() == false)
		{
			++it;
			continue;
		}

		mEntitiesToDestroy.push_back(entity);
		it = mEntities.erase(it);
	}

	//Destroy
	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it)
	{
		mCollisionSystem->Clear(*it);
		delete* it;
	}

	mEntitiesToDestroy.clear();

	//Add
	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}

	mEntitiesToAdd.clear();

	OnUpdate();
}

void Scene::FixedUpdate()
{
	//FixedUpdatable Component
	for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		Entity* entity = *it;
		entity->FixedUpdate();
	}

	//Collision
	mCollisionSystem->Update(mEntities);
}

void Scene::Draw()
{
	std::vector<Entity*> drawablesVec(mEntities.begin(), mEntities.end());

	std::sort(drawablesVec.begin(), drawablesVec.end(), [](Entity* a, Entity* b) {
		return a->GetComponent<RenderLayerComponent>()->GetLayer() < b->GetComponent<RenderLayerComponent>()->GetLayer();
		});

	for (Entity* entity : drawablesVec)
	{
		entity->Draw();
	}
}

void Scene::Exit()
{
	mCollisionSystem->ClearAll();
	mpGameManager->mStatsWindows.clear();
	OnExit();
}
