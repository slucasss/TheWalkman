#pragma once
#include "Component.h"
#include "Updatable.h"

#include "SFML/Audio.hpp"


class Sound: public Component, public Updatable{
	
	bool m_loop;

	sf::SoundBuffer m_buffer;
	sf::Sound m_sound;

	bool m_spacialize = false;

public:

	//For -1 sec sound   Only .wav or .ogg sound
	void Load(std::string path, bool loop);
	
	void Update() override;

	void SetVolume(float volume);
	void Play();
	void Pause();
	void Stop();

	void SetSpacialize(bool space);
	void SetMinDistance(float dist);

	void SetAtenuation(float factor);

};

