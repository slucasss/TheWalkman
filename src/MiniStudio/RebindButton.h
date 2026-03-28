#pragma once
#include "Button.h"
#include "KeyBinds.h"


class RebindButton: public Button{

	std::string m_bind;
	bool m_isClicked = false;
	int m_buttonType = 1; //1 = keyboard, 2 = controller
	float m_bindCooldown = 0.f;

	void OnClick();


	void Update() override;

public:
	RebindButton(std::string type): m_bind(type){
		SetOnClick([this]() { OnClick(); });
	}

	void SetButtonType(int type) {
		m_buttonType = type;
	}
	void SetBindingType(std::string type) { m_bind = type; }

};

