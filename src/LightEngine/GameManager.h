#pragma once

#include <list>

#include <vector>
#include "StatsWindow.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#define FIXED_DT 0.0041667f

class Entity;
class CollisionSystem;
class Scene;
class Debug;

namespace sf 
{
	class RenderWindow;
	class Event;
}

class GameManager
{
	sf::RenderWindow* mpWindow;
	sf::Font mFont;

	float mDeltaTime;
	float mAccumulateDt;

	int mWindowWidth;
	int mWindowHeight;


	sf::RectangleShape m_alphaTexture;
	sf::RenderTexture m_sceneTexture;
	sf::RenderTexture m_debugTexture;

	friend class Camera;
	friend class ShaderManager;

	sf::Clock m_ShaderClock;

	float timer = 0.0f;
	int FPS = 0;

private:
	GameManager();

	void Draw();

	void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }

	std::vector<StatsWindow*> mStatsWindows;
	sf::Clock mImGuiClock;

public:
	sf::Color mClearColor;

	sf::RenderWindow* GetWindow() const { return mpWindow; }

	~GameManager();
	static GameManager* Get();

	void Run();

	void CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit = 60, sf::Color clearColor = sf::Color::Black);
	StatsWindow* CreateStatsWindow(const std::string& name);


	float GetDeltaTime() const { return mDeltaTime; }
	float GetFixedDeltaTime() const { return FIXED_DT; }
	float& GetAccumulateDeltaTime() { return mAccumulateDt; }
	sf::Font& GetFont() { return mFont; };

	friend Debug;
	friend Scene;

	sf::RectangleShape* GetALphaTexture() { return &m_alphaTexture; };
	sf::RenderTexture* GetSceneTexture() { return &m_sceneTexture; };
	sf::RenderTexture* GetDebugTexture() { return &m_debugTexture; };
	sf::Clock GetShaderClock() { return m_ShaderClock; };
};