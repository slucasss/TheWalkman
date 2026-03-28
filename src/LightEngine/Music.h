#pragma once
#include "Component.h"

#include "Updatable.h"
#include "Inspectable.h"

#include "SFML/Audio.hpp"


class Music: public Component, public Updatable, public Inspectable{

	bool m_loop;

	sf::Music m_music;


	bool m_spacialize = false;

public:

	//Only .wav or .ogg sound
	void Load(std::string path, bool loop);

	void Update() override;
	void Inspect() override;

	void SetVolume(float volume);
	void Play();
	void Pause();
	void Stop();

	void SetSpacialize(bool space);
	void SetMinDistance(float dist);

	void SetAtenuation(float factor);
};

