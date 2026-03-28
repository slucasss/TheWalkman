#include "HealthBar.h"
#include "GameManager.h"
#include "imgui.h"

HealthBar::HealthBar(float width, float height)
{
    m_bg.setSize({ width, height });
    m_bg.setFillColor(sf::Color(50, 50, 50));

    m_bar.setSize({ width, height });
    m_bar.setFillColor(sf::Color::Red);

    m_maxWidth = width;
}

void HealthBar::SetHealth(float value)
{
    m_bar.setSize({ m_maxWidth * value, m_bar.getSize().y });
}

void HealthBar::Draw()
{
    sf::RenderTexture* window = GameManager::Get()->GetSceneTexture();

    sf::View world = window->getView();
    window->setView(window->getDefaultView());

    m_bg.setPosition(30, 30);
    m_bar.setPosition(30, 30);

    window->draw(m_bg);
    window->draw(m_bar);

    window->setView(world);
}

void HealthBar::Inspect()
{
    float health = m_bar.getSize().x;
    if (ImGui::SliderFloat("Health", &health, 0, m_maxWidth))
    {
        m_bar.setSize({ health, m_bar.getSize().y });
    }
}

json HealthBar::Save()
{
    return {
        {"hp", m_bar.getSize().x}
    };
}

void HealthBar::Load(const json& data)
{
    m_bar.setSize({ data["hp"], m_bar.getSize().y });
}

const char* HealthBar::GetSaveName() const
{
    return "HealthBarLogic";
}
