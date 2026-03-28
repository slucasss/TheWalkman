#pragma once
#include <map>
#include <string>
#include "Drawable.h"
#include "Updatable.h"

#define FIXED_DT 0.01667f

class Scene;
class GameManager;

class SceneManager : public Drawable, public Updatable
{
private:
	GameManager* mpGameManager;
	std::map<std::string, Scene*> m_scene;
	std::string m_currentScene;
	std::string m_nextScene;
	std::string m_previousScene = "";
	bool m_changeScene;
	SceneManager();

public:

	static SceneManager& GetInstance();
	~SceneManager();

	void SetGameManager(GameManager* gm) { mpGameManager = gm; }
	Scene* GetCurrentScene();
	std::string GetCurrentSceneID();
	void ChangeScene(const std::string& _id);
	void ReloadScene();
	void Update() override;
	void Draw() override;
	void AddScene(const std::string& _id, Scene* _scene);
	Scene* FindSceneById(const std::string& _id);
	void SetStartScene(const std::string& _id);
	void ChangeToPreviousScene();
};

