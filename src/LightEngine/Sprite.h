#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Inspectable.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Entity.h"
#include <string>

#include "Managers/ShaderManager.h"



class Sprite: public Drawable, public Component, public Inspectable{

	bool m_isVisible;
	bool m_isUI;
	sf::Sprite m_sprite;

	float m_offsetX;
	float m_offsetY;

	friend ShaderManager;
	sf::RenderStates m_renderState;
	std::string m_shaderPath = "";


public:
	void Draw() override;

	void SetTexture(std::string& path);

	Sprite(std::string path);

	void SetSrcRect(int x, int y, int width, int height);

	void SetXOffset(float x);
	void SetYOffset(float y);

	void SetOffset(sf::Vector2f offset);
	void SetTexture(const std::string& path);

	unsigned int GetHeight();
	unsigned int GetWidth();
	sf::Vector2u GetSize();

	void SetVisible(bool value) { m_isVisible = value; }
	bool IsVisible() { return m_isVisible; }

	void SetUI(bool value) { m_isUI = value; }
	bool IsUI() { return m_isUI; }

	void Inspect() override;
};

