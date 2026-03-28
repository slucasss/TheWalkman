#pragma once
#include "Button.h"
//#include ".h"

class Slider : public Button {

	float m_min = 0;
	float m_max = 100;

	sf::Color m_color = { 158, 158, 158, 0 };
	float* m_value;

	void OnClick();


	void Update() override {};

public:
	//RebindButton(float* value){
	//	SetOnClick([this]() { OnClick(); });
	//}
	
};

