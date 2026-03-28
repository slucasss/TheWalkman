
#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameManager.h"
#include "SampleScene.h"
#include "SceneManager.h"

#include <cstdlib>
#include <crtdbg.h>

int main() 
{
    GameManager* pInstance = GameManager::Get();

	pInstance->CreateWindow(1280, 720, "SampleScene", 60, sf::Color::Black);

	
	SceneManager& sm = SceneManager::GetInstance();
	sm.SetGameManager(pInstance);
	sm.AddScene("Sample", new SampleScene);

	sm.SetStartScene("Sample");

	pInstance->Run();

	return 0;
}