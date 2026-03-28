#pragma once
#include "SFML/Graphics.hpp"


class EventManager{
	static EventManager* Instance;

public:
	static EventManager* Get();

	void EventUpdate(sf::RenderWindow* window);


};

