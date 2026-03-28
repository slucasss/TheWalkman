#include "Sprite.h"

#include "Managers/AssetsManager.h"
#include "TransformComponent.h"
#include "imgui.h"

#include "GameManager.h"


Sprite::Sprite(std::string path): m_offsetX(0), m_offsetY(0){
	m_sprite.setTexture(*AssetsManager::Get()->Load(path));
	m_isVisible = true;
}

void Sprite::SetSrcRect(int x, int y, int width, int height){
	m_sprite.setTextureRect(sf::IntRect(x, y, width, height));
}

void Sprite::SetXOffset(float x){
	m_offsetX = x;
}

void Sprite::SetYOffset(float y){
	m_offsetY = y;
}

void Sprite::SetOffset(sf::Vector2f offset)
{
	m_offsetX = offset.x;
	m_offsetY = offset.y;
}

void Sprite::SetTexture(const std::string& path)
{
	m_sprite.setTexture(*AssetsManager::Get()->Load(path));
}

unsigned int Sprite::GetHeight()
{
	return m_sprite.getTexture()->getSize().y;
}

unsigned int Sprite::GetWidth()
{
	return m_sprite.getTexture()->getSize().x;
}

sf::Vector2u Sprite::GetSize()
{
	return m_sprite.getTexture()->getSize();
}

void Sprite::Inspect()
{
	ImGui::Checkbox("Visible", &m_isVisible);

	sf::Vector2f offset = { m_offsetX, m_offsetY };
	if (ImGui::DragFloat2("Offset", &offset.x, 0.1f))
		SetOffset(offset);

	ImGui::Image(m_sprite);
}

void Sprite::Draw(){
	if (m_isVisible == false)
		return;

	TransformComponent* transformComponent = m_owner->GetComponent<TransformComponent>();

	sf::Vector2f position = transformComponent->GetPosition();
	float angle = transformComponent->GetRotation();
	sf::Vector2f scale = transformComponent->GetScale();

	position.x += m_offsetX;
	position.y += m_offsetY;

	m_sprite.setOrigin(m_sprite.getLocalBounds().width * 0.5f, m_sprite.getLocalBounds().height * 0.5f);

	m_sprite.setRotation(angle);
	m_sprite.setPosition(position);
	m_sprite.setScale(scale);

	const sf::Shader* shader = m_renderState.shader;

	if (shader != nullptr && m_shaderPath != "") {
		ShaderManager::Get()->ApplyTo(this, m_shaderPath);
	}


	if (m_isUI)
	{
		sf::RenderTexture* window = GameManager::Get()->GetDebugTexture();

		sf::View world = window->getView();
		window->setView(window->getDefaultView());
		window->draw(m_sprite);
		window->setView(world);
	}
	else
	{
		sf::RenderTexture* window = GameManager::Get()->GetSceneTexture();


		window->draw(m_sprite, m_renderState);
	}

}

void Sprite::SetTexture(std::string& path) {
	m_sprite.setTexture(*AssetsManager::Get()->Load(path));
}
