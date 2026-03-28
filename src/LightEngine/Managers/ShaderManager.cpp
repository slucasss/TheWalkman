#include "ShaderManager.h"
#include "../GameManager.h"
#include "../Sprite.h"



ShaderManager* ShaderManager::Instance = nullptr;

ShaderManager* ShaderManager::Get(){
	if (Instance == nullptr) {
		Instance = new ShaderManager;
	}
	return Instance;
}

sf::Shader* ShaderManager::LoadShader(std::string path){
	if (m_shaders.find(path) == m_shaders.end()){
		m_shaders[path].loadFromFile(path, sf::Shader::Fragment);
	}
	return &m_shaders[path];
}

//Set a new current main shader
void ShaderManager::SetCurrentMainShader(std::string path) {
	if (path == "") {
		m_currentMainShader = nullptr;
		return;
	}
	else {
		m_currentMainShader = LoadShader(path);
	}
}

//Apply the CurrentMainShader to all sprites
sf::Shader* ShaderManager::Apply(){
	if (m_currentMainShader == nullptr) {
		return nullptr;
	}

	UpdateShader(m_currentMainShader);

	return m_currentMainShader;
}


//Apply the path shader to the sprite
void ShaderManager::ApplyTo(Sprite* sprite, std::string path){
	if (path == "") {
		sprite->m_renderState.shader = nullptr;
		sprite->m_shaderPath = "";
		return;
	}

	sf::Shader* shader = LoadShader(path);


	UpdateShader(shader);
	sprite->m_renderState.shader = shader;
	sprite->m_shaderPath = path;
}


void ShaderManager::UpdateShader(sf::Shader* shader) {
	shader->setUniform("texture", sf::Shader::CurrentTexture);
	shader->setUniform("time", GameManager::Get()->m_ShaderClock.getElapsedTime().asSeconds());
	shader->setUniform("resolution", sf::Vector2f{ static_cast<float>(GameManager::Get()->mWindowWidth), static_cast<float>(GameManager::Get()->mWindowHeight) });
}
