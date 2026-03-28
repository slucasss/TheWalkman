#include "RebindButton.h"
#include "Entity.h"
#include "Debug.h"
#include "TransformComponent.h"

#include "Managers/InputManager.h"
#include "GameManager.h"

#include <iostream>

void RebindButton::OnClick(){
	m_isClicked = true;
	m_bindCooldown = 0.2;
}

void RebindButton::Update(){
	Button::Update();
	if (m_isClicked == true) {
		m_bindCooldown -= GameManager::Get()->GetDeltaTime();
		if (m_buttonType == 1 ) {
			std::vector<sf::Keyboard::Key> down = InputManager::Get()->IsDownKeyboard();
			if (down.size() >= 1) {
				KeyBinds::Get()->SetBind(m_bind, KeyBinds::Get()->KeyToString(down[0]), -1 );
				m_isClicked = false;
			}
		}
		else if (m_buttonType == 2 && m_bindCooldown < 0.f) {
			std::vector<int> down = InputManager::Get()->IsHeldController();
			if (down.size() >= 1) {
				KeyBinds::Get()->SetBind(m_bind, "", down[0]);
				m_isClicked = false;
			}
		}
	}
}




