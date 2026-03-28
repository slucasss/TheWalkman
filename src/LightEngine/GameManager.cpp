#include "GameManager.h"
#include "SceneManager.h"
#include "Scene.h"

#include "Entity.h"
#include "Debug.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


#include "Managers/InputManager.h"
#include "Managers/EventManager.h"
#include "Managers/AssetsManager.h"
#include "Managers/ShaderManager.h"

#include "DataManager.h"

#include <iostream>



GameManager::GameManager()
{
	mpWindow = nullptr;
	mDeltaTime = 0.f;
	mAccumulateDt = 0.f;
	mWindowWidth = -1;
	mWindowHeight = -1;

	m_alphaTexture.setFillColor({ 0, 0, 0, 0 });
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpWindow;
	ImGui::SFML::Shutdown();
}

void GameManager::CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit, sf::Color clearColor)
{
	_ASSERT(mpWindow == nullptr);

	mpWindow = new sf::RenderWindow(sf::VideoMode(width, height), title/*, sf::Style::Fullscreen*/);
	mpWindow->setFramerateLimit(fpsLimit);
	

	mWindowWidth = width;
	mWindowHeight = height;

	mClearColor = clearColor;
	ImGui::SFML::Init(*mpWindow);
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "../../../src/LightEngine/imgui.ini";

	m_sceneTexture.create(mWindowWidth, mWindowHeight);
	m_debugTexture.create(mWindowWidth, mWindowHeight);
}

StatsWindow* GameManager::CreateStatsWindow(const std::string& name)
{
	StatsWindow* win = new StatsWindow(name);
	mStatsWindows.push_back(win);
	return win;
}

void GameManager::Run()
{
	if (mpWindow == nullptr) 
	{
		std::cout << "Window not created, creating default window" << std::endl;
		CreateWindow(1280, 720, "The walkman");
	}

	//#TODO : Load somewhere else
	bool fontLoaded = mFont.loadFromFile("../../../res/Hack-Regular.ttf");
	_ASSERT(fontLoaded); 

	sf::Clock clock;
	while (mpWindow->isOpen())
	{
		DataManager::Get().Update();

		//Update des events
		EventManager::Get()->EventUpdate(mpWindow);

		//Update des inputs
		InputManager::Get()->Update();

		SetDeltaTime(clock.restart().asSeconds());

		timer += GetDeltaTime();
		if (timer > 1.0f) {
			FPS = (int)(1.0f / GetDeltaTime());
			timer = 0.0f;
		}
		Debug::DrawText(10.0f, 10.0f, "FPS: " + std::to_string(FPS), sf::Color::White);


		//Update de la scene en cours
		SceneManager::GetInstance().Update();
		
		Draw();
	}
}

void GameManager::Draw()
{


	ImGui::SFML::Update(*mpWindow, mImGuiClock.restart());

	m_sceneTexture.clear(mClearColor);
	m_debugTexture.clear(mClearColor);
	mpWindow->clear(mClearColor);

	mpWindow->resetGLStates();

	SceneManager::GetInstance().Draw();

	for (StatsWindow* w : mStatsWindows)
		w->Draw();



	

	
	
	m_sceneTexture.display();



	sf::Sprite scene(m_sceneTexture.getTexture());
	scene.setPosition({ 0, 0 });

	if (ShaderManager::Get()->IsApply()) {
		sf::RenderStates states;
		states.shader = ShaderManager::Get()->Apply();
		mpWindow->draw(scene, states);
	}
	else {
		mpWindow->draw(scene);
	}

	ImGui::SFML::Render(m_debugTexture);

	{

		sf::View world = m_debugTexture.getView();
		m_debugTexture.setView(m_debugTexture.getDefaultView());

		Debug::Get()->Draw(&m_debugTexture);

		m_debugTexture.setView(world);
	}

	m_debugTexture.display();


	sf::Sprite debugScene(m_debugTexture.getTexture());
	debugScene.setPosition({ 0, 0 });

	mpWindow->draw(debugScene);

	m_alphaTexture.setSize(sf::Vector2f(mpWindow->getSize()));
	m_alphaTexture.setPosition(0.f, 0.f);
	mpWindow->draw(m_alphaTexture);


	mpWindow->display();
}
