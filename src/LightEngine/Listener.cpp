#include "Listener.h"
#include "Entity.h"
#include "TransformComponent.h"

#include "imgui.h"


void Listener::Update(){
	sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
	sf::Listener::setPosition(pos.x, pos.y, 0.f);
}

void Listener::Inspect(){
	sf::Vector3f pos = sf::Listener::getPosition();
	if (ImGui::DragFloat3("Position", &pos.x))
		sf::Listener::setPosition(pos);
}
