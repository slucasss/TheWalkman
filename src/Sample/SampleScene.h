#pragma once

#include "Scene.h"
#include "StatsWindow.h"

class SampleScene : public Scene
{
	Entity* pEntity1;
	Entity* pEntity2;

	float f;
	StatsWindow* test;

	float m_BeatTime = 0.0;

public:
	SampleScene();
	void OnInitialize() override;
	void OnUpdate() override;
	void OnExit() override {};
};


