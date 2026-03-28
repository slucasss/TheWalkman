#include "TextComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "GameManager.h"
#include "Managers/InputManager.h"

TextComponent::TextComponent(const std::string& fontPath, int size)
{
    m_font.loadFromFile(fontPath);

    m_text.setFont(m_font);
    m_text.setCharacterSize(size);
}

void TextComponent::SetText(const std::string& txt, bool instant, float speed)
{
	m_fullText = txt;
	m_writeSpeed = speed;

    if(instant)
    {
        m_text.setString(txt);
        m_isTyping = false;
    }
    else
    {
        m_visibleCharacters = 0;
		m_timer = 0.f;
        m_text.setString("");
		m_isTyping = true;
	}
}

void TextComponent::SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    m_text.setFillColor(sf::Color(r, g, b, a));
}

void TextComponent::Draw()
{
    if (m_isVisible == false)
        return;

    TransformComponent* transform = m_owner->GetComponent<TransformComponent>();

    m_text.setPosition(transform->GetPosition() + m_offset);
    m_text.setRotation(transform->GetRotation());

    if (m_isUI)
    {
        sf::RenderTexture* window = GameManager::Get()->GetDebugTexture();

        sf::View world = window->getView();
        window->setView(window->getDefaultView());
        window->draw(m_text);
        window->setView(world);
    }
    else
    {
        sf::RenderTexture* window = GameManager::Get()->GetSceneTexture();

        window->draw(m_text);
    }
}

void TextComponent::Update()
{
    float dt = m_owner->GetDeltaTime();

    if (!m_isTyping)
        return;

    m_timer += dt;

    float timePerChar = 1.f / m_writeSpeed;

    while (m_timer >= timePerChar && m_visibleCharacters < m_fullText.size())
    {
        m_timer -= timePerChar;
        m_visibleCharacters++;

        m_text.setString(m_fullText.substr(0, m_visibleCharacters));
    }

    if (m_visibleCharacters >= m_fullText.size())
        m_isTyping = false;

    if (InputManager::Get()->IsDown(sf::Keyboard::Space)) // TODO : Mettre la touche dans le Keybinds.ini
    {
        m_text.setString(m_fullText);
        m_isTyping = false;
    }
}