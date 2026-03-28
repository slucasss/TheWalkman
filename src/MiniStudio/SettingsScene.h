#pragma once

#include "Scene.h"

class SettingsScene: public Scene {

	float m_cursorMovementTimer;

	void OnInitialize() override;
	void OnLoading() override;

	void OnUpdate() override;
	void OnExit() override;
};