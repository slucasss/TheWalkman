#include "Button.h"



#include "Managers/InputManager.h"
#include "Utils.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "Debug.h"

#include <iostream>

#include "Sprite.h"

void Button::Update(){
	if (m_activate == false) {
		return;
	}
	InputManager* input = InputManager::Get();

	sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
	sf::Vector2f scale = m_owner->GetComponent<TransformComponent>()->GetScale();
	sf::Vector2f mousePos = input->GetMousePosition();
	if ((mousePos.x > pos.x && mousePos.x < pos.x + (m_size.x * scale.x) && mousePos.y > pos.y && mousePos.y < pos.y + (m_size.y * scale.x) || m_isControllerSelected)){
		if (m_selectedPath!= "") {
			m_owner->GetComponent<Sprite>()->SetTexture(m_selectedPath);
		}

		if (m_MouseOn != nullptr) {
			m_MouseOn();
		}
		if (input->IsDown(sf::Mouse::Left) || InputManager::Get()->IsDown(1)) {
		
			if (m_OnClick != nullptr) {
				m_OnClick();
			}
			if (m_onClickWithArg!= nullptr) {
				m_onClickWithArg(m_arg);
			}
		}
	}
	else {
		if (m_path != "") {
			m_owner->GetComponent<Sprite>()->SetTexture(m_path);
		}
	}
	
}


void Button::Draw() {
	sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
	sf::Vector2f scale = m_owner->GetComponent<TransformComponent>()->GetScale();

	//Debug::Get()->DrawRectangle(pos.x, pos.y, m_size.x * scale.x, m_size.y * scale.y, sf::Color::Red);

	if (m_isControllerSelected && InputManager::Get()->IsControllerConnected()) {
		Debug::Get()->DrawRectangle(pos.x, pos.y, m_size.x * scale.x, m_size.y * scale.y, sf::Color::Green);
	}
}

void Button::SetPath(std::string path){
	m_path = path;
}

void Button::SetSelectedPath(std::string path){
	m_selectedPath = path;
}
