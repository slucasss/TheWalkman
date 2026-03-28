#pragma once
#include "Scene.h"


class DeathMenuScene: public Scene{
	float m_cursorMovementTimer = 0.f;

public:

	void OnInitialize() override;
	void OnLoading() override;

	void OnUpdate() override;
	void OnExit() override;
};

