#pragma once
#include "Component.h"
#include "Drawable.h"
#include "Updatable.h"
#include <SFML/Graphics.hpp>
#include <string>

class TextComponent : public Component, public Drawable, public Updatable
{
public:
    TextComponent(const std::string& fontPath, int size);

    void SetText(const std::string& txt, bool instant = true, float speed = 30.f);
    void SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void SetOffset(sf::Vector2f offset) { m_offset = offset; }
    void SetVisible(bool visible) { m_isVisible = visible; }

    void Draw() override;
	void Update() override;

	bool IsTyping() const { return m_isTyping; }
    bool IsVisible() const { return m_isVisible; }


    void SetIsUI(bool ui) { m_isUI = ui; }
    bool IsUI() { return m_isUI; }
private:

    bool m_isUI;

    sf::Text m_text;
    sf::Font m_font;

	std::string m_fullText;

	size_t m_visibleCharacters = 0;

	float m_writeSpeed;
    float m_timer = 0.f;

	bool m_isTyping = false;
    bool m_isVisible = true;

    sf::Vector2f m_offset{ 0.f, 0.f };

};