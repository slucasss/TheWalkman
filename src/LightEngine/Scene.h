#pragma once

class GameManager;
class Entity;

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>

#include <list>
#include <vector>

#include "Updatable.h"
#include "Drawable.h"
#include "FixedUpdatable.h"

class CollisionSystem;
class TilemapLoader;

class Scene : public Updatable, public FixedUpdatable, public Drawable
{
protected:
	GameManager* mpGameManager;
	CollisionSystem* mCollisionSystem;
	TilemapLoader* m_loader;

	std::list<Entity*> mEntities;
	std::list<Entity*> mEntitiesToDestroy;
	std::list<Entity*> mEntitiesToAdd;

	bool m_initialize = true;

private:
	void SetGameManager(GameManager* pGameManager) { mpGameManager = pGameManager; }

	void Initialize();
	void Update() override;
	void FixedUpdate() override;
	void Draw() override;
	void Exit();

protected:
	Scene();
	virtual ~Scene();

	virtual void OnInitialize() = 0;
	virtual void OnLoading() {};

	virtual void OnUpdate() = 0;
	virtual void OnExit() = 0;
	

public:
	Entity* CreateEntity();

	Entity* CreateEntityNow();

	float GetDeltaTime() const;
	float GetFixedDeltaTime() const;

	int GetWindowWidth() const;
	int GetWindowHeight() const;

	void DestroyEntityNow(Entity* _ent);
	void DestroyAllEntities();
	void DestroyEntityWithTag(const std::string& _tag);
	void DestroyAllEntitiesWithTag(const std::string& _tag);

	Entity* FindEntityWithTag(const std::string& _tag);
	std::vector<Entity*> FindAllEntitiesWithTag(const std::string& _tag);

	void SetInitalize(bool initialize) { m_initialize = initialize; }
	Entity* FindEntityByID(uint64_t id);

	void Save(const std::string& path);
	void Load(const std::string& path);

	friend GameManager;
	friend class SceneManager;
};