#include <SFML/Graphics.hpp>


#include "GameManager.h"
#include "StartScene.h"
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "SettingsScene.h"
#include "PauseScene.h"
#include "DeathMenuScene.h"
#include "EndMenu.h"

int main()
{
	srand(NULL);

	GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1280,720,"The Walkman",60, sf::Color{ 255, 248, 207, 255 });

	SceneManager& sm = SceneManager::GetInstance();
	sm.SetGameManager(pInstance);

	sm.AddScene("Start", new StartScene);
	sm.AddScene("MainMenu", new MainMenuScene);
	sm.AddScene("Settings", new SettingsScene);
	sm.AddScene("Pause", new PauseScene);
	sm.AddScene("Death", new DeathMenuScene);
	sm.AddScene("End", new EndMenu);

	sm.SetStartScene("MainMenu");
	

	pInstance->Run();
	return 0;
}
