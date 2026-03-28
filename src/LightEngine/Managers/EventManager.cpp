 #include "EventManager.h"
#include "InputManager.h"

#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "../imgui-SFML.h"
#include "../GameManager.h"
#include <iostream>


EventManager* EventManager::Instance = nullptr;

EventManager* EventManager::Get() {
	if (Instance == nullptr) {
		Instance = new EventManager;
	}
	return Instance;
}

void EventManager::EventUpdate(sf::RenderWindow* window){
	for (int i = 0; i < 8; i++) {
		if (sf::Joystick::isConnected(0)) {
			InputManager::Get()->m_connectedController = true;
		}
	}
	

	if(InputManager::Get()->IsControllerConnected()){
		window->setMouseCursorVisible(false);
	}
	else {
		window->setMouseCursorVisible(true);
	}



	sf::Event event;

	while(window->pollEvent(event)){

		//ImGui events
		ImGui::SFML::ProcessEvent(event);

		//Window events
		if(event.type == sf::Event::Closed){
			window->close();
		}

		//Keyboard events
		else if (event.type == sf::Event::KeyPressed) {
			InputManager::Get()->m_keyboardState[static_cast<size_t>(event.key.code)].isDown = true;
			InputManager::Get()->m_keyboardState[static_cast<size_t>(event.key.code)].isUp = false;
		}
		else if (event.type == sf::Event::KeyReleased) {
			InputManager::Get()->m_keyboardState[static_cast<size_t>(event.key.code)].isUp = true;
		}

		//Mouse events
		else if (event.type == sf::Event::MouseButtonPressed) {
			InputManager::Get()->m_mouseState[static_cast<size_t>(event.mouseButton.button)].isMouseDown = true;
			InputManager::Get()->m_mouseState[static_cast<size_t>(event.mouseButton.button)].isMouseUp = false;

		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			InputManager::Get()->m_mouseState[static_cast<size_t>(event.mouseButton.button)].isMouseUp = true;
		}


		//Mouse event
		else if (event.type == sf::Event::MouseMoved) {
			sf::RenderTexture* window = GameManager::Get()->GetSceneTexture();
			sf::Vector2i pos{ event.mouseMove.x,event.mouseMove.y };

			sf::Vector2f worldPos = window->mapPixelToCoords(pos);

			InputManager::Get()->m_mouseX = worldPos.x;
			InputManager::Get()->m_mouseY = worldPos.y;
		}

		//Controller events
		else if (InputManager::Get()->IsControllerConnected()) {
			InputManager* input = InputManager::Get();

			if (event.type == sf::Event::JoystickMoved) {
				if (event.joystickMove.axis == sf::Joystick::Axis::X) {
					input->m_leftJoystick.x = event.joystickMove.position;
				}
				if (event.joystickMove.axis == sf::Joystick::Axis::Y) {
					input->m_leftJoystick.y = event.joystickMove.position;
				}
				if (event.joystickMove.axis == sf::Joystick::Axis::Z) {
					input->m_rightJoystick.x = event.joystickMove.position;
				}
				if (event.joystickMove.axis == sf::Joystick::Axis::R) {
					input->m_rightJoystick.y = event.joystickMove.position;
				}
			}

			else if (event.type == sf::Event::JoystickButtonPressed) {
				input->m_controllerState[event.joystickButton.button].isButtonDown = true;
				input->m_controllerState[event.joystickButton.button].isButtonUp = false;
			}
			else if (event.type == sf::Event::JoystickButtonReleased) {
				input->m_controllerState[event.joystickButton.button].isButtonUp = true;
			}
		}

		
		
		

	}
}

