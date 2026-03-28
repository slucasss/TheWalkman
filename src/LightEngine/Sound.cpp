#include "Sound.h"
#include <iostream>

#include "Entity.h"
#include "TransformComponent.h"


void Sound::Load(std::string path, bool loop){
	if (!m_buffer.loadFromFile(path)) {
		std::cout << "loading sound " << path << " error" << std::endl;
		return;
	}

	m_sound.setBuffer(m_buffer);
	m_loop = loop;
	m_sound.setLoop(m_loop);
	m_sound.setVolume(100);
}

void Sound::Update(){
	if (m_spacialize) {
		sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
		m_sound.setPosition(pos.x, pos.y, 0.f);
	}
}

void Sound::SetVolume(float volume){
	m_sound.setVolume(volume);
}

void Sound::Play(){
	m_sound.play();
}

void Sound::Pause(){
	m_sound.pause();
}

void Sound::Stop(){
	m_sound.stop();
}

void Sound::SetSpacialize(bool space){
	m_spacialize = space;
	m_sound.setRelativeToListener(!space);
}

void Sound::SetMinDistance(float dist){
	m_sound.setMinDistance(dist);
}

void Sound::SetAtenuation(float factor){
	m_sound.setAttenuation(factor);
}


