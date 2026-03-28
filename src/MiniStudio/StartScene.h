#pragma once

#include "Scene.h"

class StatsWindow;
class PropAtlas;
class RhythmSpawner;

class StartScene : public Scene
{
	StatsWindow* PlayerStats;
	StatsWindow* BossStats;
	StatsWindow* RythmStats;
	StatsWindow* hypebar;

	PropAtlas* atlas;

	RhythmSpawner* m_rhythmSpawner;

public:

	StartScene();
	~StartScene();
	Entity* CreateRythmEntities(sf::Vector2f pos);
	Entity* CreateHypeBar(sf::Vector2f pos);
	void OnInitialize() override;
	void OnLoading() override;

	void OnUpdate() override;
	void OnExit() override {};
	Entity* CreateMusicChangeUi(int x, int y);
	Entity* CreateHealth(int x, int y);
	Entity* CreateRat(int x, int y);
	Entity* CreatePlayer();
	Entity* CreateSlot(int index);
	Entity* CreateBat(int x, int y);

	Entity* CreateCollectible(const std::string& path, float x, float y, float rotation, sf::Vector2f size);
	Entity* CreateConsommable(const std::string& path, const std::string& tag, float x, float y, sf::Vector2f scale);
	Entity* CreateNPC(std::string path, float x, float y, std::string dialogue, sf::Vector2f scale);
};