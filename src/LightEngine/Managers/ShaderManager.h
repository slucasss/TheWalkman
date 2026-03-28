#pragma once

#include "SFML/Graphics.hpp"

#include "map"

class Sprite;

class ShaderManager {
	static ShaderManager* Instance;

	std::map<std::string, sf::Shader> m_shaders;

	bool m_visible = false;
	sf::Shader* m_currentMainShader;

	friend class GameManager;
public:

	static ShaderManager* Get();

	sf::Shader* LoadShader(std::string path);
	sf::Shader* Apply();

	void SetVisible(bool visible) { m_visible = visible; };
	bool IsApply() { return m_visible; };

	template <typename T>
	void SetParameter(std::string path, const char* name, T value) {
		if (m_shaders.find(path) == m_shaders.end()) {
			return;
		}
		m_shaders[path].setUniform(name, value);
	}

	void ApplyTo(Sprite* sprite, std::string path);

	void SetCurrentMainShader(std::string path);
	sf::Shader* GetCurrentMainShader() {
		return m_currentMainShader;
	}


	void UpdateShader(sf::Shader* shader);

};
