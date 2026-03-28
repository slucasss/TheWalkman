#pragma once
#include "SFML/Audio.hpp"
#include "Component.h"
#include "Updatable.h"
#include "Inspectable.h"


class Listener: public Component, public Updatable, public Inspectable{
public:
	void Update() override;

	void Inspect() override;
};

