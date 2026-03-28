#pragma once
#include <SFML/Window.hpp>


class InputManager{

	struct State {
		bool isDown;
		bool isHeld;
		bool isUp;
	};

	struct MouseState {
		bool isMouseDown;
		bool isMouseHeld;
		bool isMouseUp;
	};

	struct ButtonState {
		bool isButtonDown;
		bool isButtonHeld;
		bool isButtonUp;
	};


	static InputManager* Instance;

	State m_keyboardState[sf::Keyboard::KeyCount];
	MouseState m_mouseState[sf::Mouse::ButtonCount];
	ButtonState m_controllerState[sf::Joystick::ButtonCount];


	float m_mouseX = 0.f;
	float m_mouseY = 0.f;

	sf::Vector2f m_rightJoystick;
	sf::Vector2f m_leftJoystick;

	InputManager();

	bool m_connectedController;

	friend class EventManager;

public:
	static InputManager* Get();


	//Update all the keys in the keys Table
	void Update();

	//Keyboard
	bool IsHeld(sf::Keyboard::Key);
	bool IsDown(sf::Keyboard::Key);
	bool IsUp(sf::Keyboard::Key);


	//Mousse
	bool IsHeld(sf::Mouse::Button);
	bool IsDown(sf::Mouse::Button);
	bool IsUp(sf::Mouse::Button);
	
	sf::Vector2f GetMousePosition();



	//Controller
	bool IsHeld(int key);
	bool IsDown(int key);
	bool IsUp(int key);

	sf::Vector2f GetLeftJoystick();
	sf::Vector2f GetRightJoystick();

	std::vector<sf::Keyboard::Key> IsHeldKeyboard();
	std::vector<int> IsHeldController();

	std::vector<sf::Keyboard::Key> IsDownKeyboard();

	std::vector<int> IsDownController();

	bool IsControllerConnected() { return m_connectedController; }
};

