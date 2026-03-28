#include "InputManager.h"
#include "../GameManager.h"
#include <iostream>

InputManager* InputManager::Instance = nullptr;

InputManager::InputManager(){
	for (State& state : m_keyboardState) {
		state.isDown = false;
		state.isHeld = false;
		state.isUp = false;
	}

	for (MouseState& state : m_mouseState) {
		state.isMouseDown = false;
		state.isMouseHeld = false;
		state.isMouseUp = false;
	}
	m_mouseX = sf::Mouse::getPosition().x;
	m_mouseY = sf::Mouse::getPosition().y;

}

InputManager* InputManager::Get() {
	if (Instance == nullptr) {
		Instance = new InputManager;
	}
	return Instance;
}

void InputManager::Update(){
	for (State& state : m_keyboardState)
	{
		if (state.isHeld) {
			state.isDown = false;
		}
		if (state.isDown) {
			state.isHeld = true;
		}
		if (state.isUp) {
			state.isHeld = false;
			state.isDown = false;
		}
	}

	for (MouseState& state : m_mouseState)
	{
		if (state.isMouseHeld) {
			state.isMouseDown = false;
		}
		if (state.isMouseDown){
			state.isMouseHeld = true;
		}
		if (state.isMouseUp){
			state.isMouseHeld = false;
			state.isMouseDown = false;
		}
	}

	for (ButtonState& state : m_controllerState)
	{
		if (state.isButtonHeld) {
			state.isButtonDown = false;
		}
		if (state.isButtonDown){
			state.isButtonHeld = true;
		}
		if (state.isButtonUp){
			state.isButtonHeld = false;
			state.isButtonDown = false;
		}
	}

	m_connectedController = sf::Joystick::isConnected(0);
}

bool InputManager::IsHeld(sf::Keyboard::Key key){
	return m_keyboardState[static_cast<int>(key)].isHeld;
}

bool InputManager::IsDown(sf::Keyboard::Key key){
	return m_keyboardState[static_cast<int>(key)].isDown;
}

bool InputManager::IsUp(sf::Keyboard::Key key){
	return m_keyboardState[static_cast<int>(key)].isUp;
}

bool InputManager::IsHeld(sf::Mouse::Button key){
	return m_mouseState[static_cast<int>(key)].isMouseHeld;
}

bool InputManager::IsDown(sf::Mouse::Button key){
	return m_mouseState[static_cast<int>(key)].isMouseDown;
}

bool InputManager::IsUp(sf::Mouse::Button key){
	return m_mouseState[static_cast<int>(key)].isMouseUp;
}

sf::Vector2f InputManager::GetMousePosition(){
	return sf::Vector2f{m_mouseX, m_mouseY};
}

bool InputManager::IsHeld(int key){
	return m_controllerState[key].isButtonHeld;
}

bool InputManager::IsDown(int key){
	return m_controllerState[key].isButtonDown;
}

bool InputManager::IsUp(int key){
	return m_controllerState[key].isButtonUp;
}

sf::Vector2f InputManager::GetLeftJoystick(){
	return m_leftJoystick;
}

sf::Vector2f InputManager::GetRightJoystick(){
	return m_rightJoystick;
}

std::vector<sf::Keyboard::Key> InputManager::IsHeldKeyboard() {
	std::vector<sf::Keyboard::Key> m_KeysDown;
	for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
		if (m_keyboardState[i].isHeld == true){
			m_KeysDown.push_back(static_cast<sf::Keyboard::Key>(i));
			break;
		}
	}
	return m_KeysDown;
}

std::vector<int> InputManager::IsHeldController(){
	std::vector<int> m_ButtonDown;
	for (int i = 0; i < sf::Joystick::ButtonCount; i++) {
		if (m_controllerState[i].isButtonHeld == true) {
			m_ButtonDown.push_back(i);
			break;
		}
	}
	return m_ButtonDown;
}
std::vector<sf::Keyboard::Key> InputManager::IsDownKeyboard() {
	std::vector<sf::Keyboard::Key> m_KeysDown;
	for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
		if (m_keyboardState[i].isDown == true){
			m_KeysDown.push_back(static_cast<sf::Keyboard::Key>(i));
			break;
		}
	}
	return m_KeysDown;
}

std::vector<int> InputManager::IsDownController(){
	std::vector<int> m_ButtonDown;
	for (int i = 0; i < sf::Joystick::ButtonCount; i++) {
		if (m_controllerState[i].isButtonDown == true) {
			m_ButtonDown.push_back(i);
			break;
		}
	}
	return m_ButtonDown;
}
