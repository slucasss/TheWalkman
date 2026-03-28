#include "Music.h"
#include <iostream>

#include "Entity.h"
#include "TransformComponent.h"

void Music::Load(std::string path, bool loop){
	if (!m_music.openFromFile(path)) {
		std::cout << "loading music " << path << " error" << std::endl;
		return;
	}

	m_music.setLoop(loop);
	m_music.setVolume(50);
}

void Music::Update(){
	if (m_spacialize) {
		sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
		m_music.setPosition(pos.x, pos.y, 0.f);

	}
}

void Music::Inspect(){

}

void Music::SetVolume(float volume){
	m_music.setVolume(volume);
}

void Music::Play(){
	m_music.play();
}

void Music::Pause(){
	m_music.pause();
}

void Music::Stop(){
	m_music.stop();
}

void Music::SetSpacialize(bool space) {
	m_spacialize = space;
	m_music.setRelativeToListener(!space);
}

void Music::SetMinDistance(float dist) {
	m_music.setMinDistance(dist);
}

void Music::SetAtenuation(float factor) {
	m_music.setAttenuation(factor);
}