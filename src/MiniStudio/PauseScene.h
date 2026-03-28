#pragma once

#include "Scene.h"
#include "CircleCollider.h"

#include <functional>

class StatsWindow;

class PauseScene : public Scene{

	struct Buttons {
		Entity* entity;
		float angleOffset;
		std::function<void()> execution;
	};

	std::vector<Buttons> m_buttons;

	float m_disqueRadius = 300;

	float m_currentAngle = 0.f;   
	float m_targetAngle = 0.f;  

	sf::Vector2f m_center = { 700, 320 };
	bool m_isTurning = false;

	int id;

	float m_cursorMovementTimer;

public:

	void Turn(Entity*);

	PauseScene();

	void OnInitialize() override;
	void OnLoading() override;

	void OnUpdate() override;
	void OnExit() override;
};

