#include "SceneManager.h"
#include "Scene.h"
#include "CollisionSystem.h"
#include "Managers/EventManager.h"
#include "GameManager.h"

#include <thread>
#include <atomic>



SceneManager::SceneManager()
{
	mpGameManager = nullptr;
	m_changeScene = true;
}

SceneManager& SceneManager::GetInstance()
{
	static SceneManager Instance;
	return Instance;
}

SceneManager::~SceneManager()
{
	for (auto& pair : m_scene)
	{
		delete pair.second;
	}
}

Scene* SceneManager::GetCurrentScene()
{
	return m_scene[m_currentScene];
}

std::string SceneManager::GetCurrentSceneID()
{
	return m_currentScene;
}

void SceneManager::ChangeScene(const std::string& _id)
{
	m_previousScene = m_currentScene;
	m_nextScene = _id;
	m_changeScene = true;
	GameManager::Get()->GetSceneTexture()->setView(GameManager::Get()->GetSceneTexture()->getDefaultView());
}

void SceneManager::ReloadScene()
{
	m_changeScene = true;
}

void SceneManager::Update()
{

	if (m_changeScene)
	{
		m_scene[m_currentScene]->Exit();

		m_currentScene = m_nextScene;
		if (m_scene[m_currentScene]->m_initialize) {
			std::atomic<bool>initDone(false);

			std::thread initThread([&]() {
				sf::Context context;
				m_scene[m_currentScene]->Initialize();
				initDone = true;
			});

			while (initDone == false) {
				std::this_thread::sleep_for(std::chrono::milliseconds(16));
				EventManager::Get()->EventUpdate(GameManager::Get()->GetWindow());
				sf::RenderWindow* window = GameManager::Get()->GetWindow();
				
				window->clear();
				m_scene[m_currentScene]->OnLoading();
				window->display();
				sf::View view = GameManager::Get()->GetSceneTexture()->getView();

			}
			initThread.join();
		}
		m_changeScene = false;
		m_scene[m_currentScene]->m_initialize = false;
	}

	m_scene[m_currentScene]->Update();
}
 
void SceneManager::Draw()
{
	m_scene[m_currentScene]->Draw();
}

void SceneManager::AddScene(const std::string& _id, Scene* _scene)
{
	_scene->SetGameManager(mpGameManager);
	m_scene[_id] = _scene;
}

Scene* SceneManager::FindSceneById(const std::string& _id)
{
	return m_scene[_id];
}

void SceneManager::SetStartScene(const std::string& _id)
{
	m_currentScene = _id;
	m_nextScene = _id;
}

void SceneManager::ChangeToPreviousScene(){
	if (m_previousScene == "") {
		return;
	}
	FindSceneById(m_previousScene)->SetInitalize(true);

	m_nextScene = m_previousScene;
	m_previousScene = m_currentScene;

	m_changeScene = true;
	GameManager::Get()->GetSceneTexture()->setView(GameManager::Get()->GetSceneTexture()->getDefaultView());
}



