#pragma once
#include "Scene.h"
class MainMenuScene: public Scene{

	int alpha = 0;
	float m_cursorMovementTimer;

public:
	void OnInitialize() override;
	void OnLoading() override;

	void OnUpdate() override;
	void OnExit() override ;
};

