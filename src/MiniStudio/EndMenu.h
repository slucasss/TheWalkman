#pragma once
#include "Scene.h"


class EndMenu: public Scene{

	void OnInitialize() override;
	void OnLoading() override;

	void OnUpdate() override;
	void OnExit() override;

	std::string m_endType = "loose";

public:


	void SetEndType(const std::string& type);

};

