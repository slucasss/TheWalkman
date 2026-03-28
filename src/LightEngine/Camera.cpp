#include "Camera.h"
#include "TransformComponent.h"
#include "GameManager.h"
#include "Utils.h"

#include "imgui.h"
#include <iostream>

#include "SceneManager.h"
#include "Scene.h"

#include "TagComponent.h"

Camera::Camera() : m_link(nullptr) {
	m_viewSize.x = 1280;
	m_viewSize.y = 720;

	//m_view.setCenter(500, 100);
	m_view.setSize(m_viewSize);

	m_offset.y = 0.f;

}

void Camera::TransitionTo(sf::FloatRect newView, float resizeSpeed, float movementSpeed) {
	m_GoToPosition = sf::Vector2f{ newView.left, newView.top };
	m_GoToSize = sf::Vector2f{ newView.width, newView.height };
	m_haveToMove = true;
	m_HaveToResize = true;
	m_moveSpeed = movementSpeed;
	m_resizeSpeed = resizeSpeed;
	m_followEntity = false;
	m_isLink = false;
}

void Camera::TransitionTo(Entity* entity, sf::Vector2f newSize, float resizeSpeed, float movementSpeed) {
	m_GoToPosition = sf::Vector2f{ entity->GetComponent<TransformComponent>()->GetPosition() };


	if (newSize == sf::Vector2f{ -1, -1 }) {
		m_GoToSize = m_view.getSize();
	}
	else {
		m_GoToSize = sf::Vector2f{ newSize.x, newSize.y};
	}

	m_haveToMove = true;
	m_HaveToResize = true;
	m_moveSpeed = movementSpeed;
	m_resizeSpeed = resizeSpeed;
	m_followEntity = true;
	m_link = entity;
	m_isLink = false;
}

void Camera::SetLink(Entity* link) {
	if (link == nullptr) {
		return;
	}

	m_link = link;
	m_isLink = true;

	sf::Vector2f linkPos = m_link->GetComponent<TransformComponent>()->GetPosition();
	m_owner->GetComponent<TransformComponent>()->SetPosition(linkPos);

	sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
	m_view.setCenter(pos.x + m_offset.x, pos.y + m_offset.y);
}

void Camera::Update() {
	if (m_isMainCam == false) {
		return;
	}


	if (m_isLink == false) {
		if (m_haveToMove) {
			TransformComponent* transform = m_owner->GetComponent<TransformComponent>();
			if (m_followEntity == true) {
				m_GoToPosition = m_link->GetComponent<TransformComponent>()->GetPosition();
			}

			sf::Vector2f newPos = sf::Vector2f{ m_GoToPosition.x, m_GoToPosition.y };
			sf::Vector2f currentPos = transform->GetPosition();

			if (newPos != currentPos) {
				sf::Vector2f vector = newPos - currentPos;
				float dt = GameManager::Get()->GetDeltaTime();
				if (std::hypot(vector.x, vector.y) > m_moveSpeed * dt) {

					sf::Vector2f& dir = vector;
					Utils::Normalize(dir);
					transform->SetPosition({ currentPos.x + ((dir.x * m_moveSpeed) * dt), currentPos.y + ((dir.y * m_moveSpeed) * dt) });
				
				}
				else {
					transform->SetPosition(m_GoToPosition);
					m_haveToMove = false;
					if (m_followEntity == true) {
						m_isLink = true;
					}
				}

			}
		}
		
		if (m_HaveToResize) {
			sf::Vector2f newSize = sf::Vector2f{ m_GoToSize.x, m_GoToSize.y };
			if (newSize != m_viewSize && m_HaveToResize) {
				sf::Vector2f vector = newSize - m_viewSize;
				float dt = GameManager::Get()->GetDeltaTime();
				if (std::hypot(vector.x, vector.y) > m_resizeSpeed * dt) {

					sf::Vector2f& dir = vector;
					Utils::Normalize(dir);


					m_viewSize.x = m_viewSize.x + ((dir.x * m_resizeSpeed) * dt);
					m_viewSize.y = m_viewSize.y + ((dir.y * m_resizeSpeed) * dt);
				}
				else {
					m_viewSize = m_GoToSize;
					m_HaveToResize = false;
					
				}
			}
		}
	}


	if (m_smoothMovement == false && m_link != nullptr && m_isLink == true) {
		sf::Vector2f linkPos = m_link->GetComponent<TransformComponent>()->GetPosition();

		m_owner->GetComponent<TransformComponent>()->SetPosition(linkPos.x + m_offset.x, linkPos.y + m_offset.y);
	}


	else if(m_smoothMovement == true && m_link != nullptr && m_isLink == true){
		sf::Vector2f cameraPos = m_owner->GetComponent<TransformComponent>()->GetPosition();
		sf::Vector2f pos = m_link->GetComponent<TransformComponent>()->GetPosition();

		if ((pos.x + m_offset.x) > cameraPos.x) {
			if ((pos.x + m_offset.x) - cameraPos.x >= 150) {
				cameraPos.x = pos.x + m_offset.x - 150;
				m_cameraXSpeed = 0;
				m_hasMooved = true;
			}
			
			else if ((pos.x + m_offset.x) - cameraPos.x >= 30) {
				m_cameraXSpeed += 50;
				m_hasMooved = true;
			}


			if (pos.x == previousLinkCoordinates.x && m_hasMooved == true){
				if (m_cameraXSpeed <= 0) {
					m_cameraXSpeed = 100;
				}
			}
			else if(pos.x != previousLinkCoordinates.x && m_hasMooved == true){
				m_hasMooved = false;
				m_cameraXSpeed = 0;
			}



			if (m_hasMooved == true && (pos.x + m_offset.x) <= (cameraPos.x + m_cameraXSpeed * GameManager::Get()->GetDeltaTime())) {
				cameraPos.x = pos.x + m_offset.x;
				m_cameraXSpeed = 0;
				m_hasMooved = false;
			}
		}
		
		else if((pos.x + m_offset.x) < cameraPos.x){
			if ((pos.x + m_offset.x) - cameraPos.x <= -150) {
				cameraPos.x = pos.x + m_offset.x + 150;
				m_cameraXSpeed = 0;
				m_hasMooved = true;
			}
			else if ((pos.x + m_offset.x) - cameraPos.x <= -30) {
				m_cameraXSpeed = m_cameraXSpeed - 50;
				m_hasMooved = true;
			}



			if (pos.x == previousLinkCoordinates.x && m_hasMooved == true) {
				if (m_cameraXSpeed >= 0) {
					m_cameraXSpeed = -100;
				}
			}
			else if (pos.x != previousLinkCoordinates.x && m_hasMooved == true) {
				m_hasMooved = false;
				m_cameraXSpeed = 0;
			}



			if (m_hasMooved == true && (pos.x + m_offset.x) >= (cameraPos.x + m_cameraXSpeed * GameManager::Get()->GetDeltaTime())) {
				cameraPos.x = pos.x + m_offset.x;
				m_cameraXSpeed = 0;
				m_hasMooved = false;
			}
		}
	


		previousCoordinates = cameraPos;
		previousLinkCoordinates = pos;
		cameraPos.x += m_cameraXSpeed * GameManager::Get()->GetDeltaTime();
		cameraPos.y = pos.y;

		m_owner->GetComponent<TransformComponent>()->SetPosition(cameraPos);

	}
	
	m_owner->GetComponent<TransformComponent>()->SetPosition(m_owner->GetComponent<TransformComponent>()->GetPosition() + m_offset);
	m_view.setCenter(m_owner->GetComponent<TransformComponent>()->GetPosition());
	m_view.setSize(m_viewSize);

	GameManager::Get()->GetSceneTexture()->setView(m_view);
	GameManager::Get()->GetDebugTexture()->setView(m_view);



	if (m_isFadingOut == true){
		sf::RectangleShape* shape = GameManager::Get()->GetALphaTexture();
		sf::Color color = shape->getFillColor();

		float time = m_fadeOutSpeed;
		shape->setSize(sf::Vector2f{ static_cast<float>(GameManager::Get()->mWindowWidth),static_cast<float>(GameManager::Get()->mWindowHeight) });
		sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();


		sf::Vector2f RectPos{ pos.x - (m_viewSize.x / 2), pos.y - (m_viewSize.y / 2) };
		shape->setPosition(RectPos);


		if (color.a == 255) {
			m_isFadingOut = false;
			return;
		}

		if (time == 0) {
			color.a = 255;
			shape->setFillColor(color);
		}
		else {
			int plus = (255 / time) * GameManager::Get()->GetDeltaTime();

			if (color.a + plus >= 255) {
				color.a = 255;
				shape->setFillColor(color);
			}
			else {
				color.a += plus;
				shape->setFillColor(color);
			}
		}
	}

	if (m_isFadingIn == true) {
		sf::RectangleShape* shape = GameManager::Get()->GetALphaTexture();
		sf::Color color = shape->getFillColor();


		if (color.a == 0) {
			m_isFadingIn = false;
			return;
		}


		shape->setSize(sf::Vector2f{ static_cast<float>(GameManager::Get()->mWindowWidth),static_cast<float>(GameManager::Get()->mWindowHeight) });
		sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();


		sf::Vector2f RectPos{ pos.x - (m_viewSize.x / 2), pos.y - (m_viewSize.y / 2) };
		shape->setPosition(RectPos);
		float time = m_fadeInSpeed;

		if (time == 0) {
			color.a = 0;
			shape->setFillColor(color);
		}

		else {
			int minus = (255 / time) * GameManager::Get()->GetDeltaTime();

			if (color.a - minus <= 0) {
				color.a = 0;
				shape->setFillColor(color);
			}
			else {
				color.a -= minus;
				shape->setFillColor(color);
			}
		}
	}


	if (m_shakingDuration != 0.f) {
		m_shakingDuration -= GameManager::Get()->GetDeltaTime();
		m_shakeTimer += GameManager::Get()->GetDeltaTime();

		if (m_shakeTimer >= 0.1) {
			m_offset.y = m_offset.y + (m_shakMultiplicator * m_shakingIntensity);
			m_shakeTimer -= 0.05;
			m_shakMultiplicator = m_shakMultiplicator * -1;
		}
		if (m_shakingDuration <= 0.f) {
			m_shakingDuration = 0.f;
			m_offset.y = m_initialOffset;
		}
	}
}

void Camera::SetMainCamera(){
	std::vector<Entity*> cameras = SceneManager::GetInstance().GetCurrentScene()->FindAllEntitiesWithTag("camera");
	for (auto c : cameras) {
		c->GetComponent<Camera>()->m_isMainCam = false;
	}
	m_isMainCam = true;

	GameManager::Get()->GetSceneTexture()->setView(m_view);
	GameManager::Get()->GetDebugTexture()->setView(m_view);
}

void Camera::FixCamera(){
	m_isLink = false;

	sf::Vector2f pos = m_owner->GetComponent<TransformComponent>()->GetPosition();
	m_view.setCenter({ pos.x + m_offset.x, pos.y + m_offset.y });
	m_view.setSize(m_viewSize);
}

void Camera::SetViewSize(sf::Vector2f size){
	m_viewSize = size;
}

void Camera::SetOffset(sf::Vector2f offset) {
	m_offset = offset;
}

void Camera::Inspect()
{
	ImGui::DragFloat2("ViewSize", &m_viewSize.x);
	ImGui::DragFloat2("Offset", &m_offset.x);

	sf::Vector2f pos = m_view.getCenter();

	if (ImGui::DragFloat2("View Center Position", &pos.x))
		m_view.setCenter(pos);

	ImGui::Checkbox("Camera Fix", &m_isLink);
}

void Camera::Rotate(float rotation){
	m_view.rotate(rotation);
}

void Camera::FadeOut(float time){
	m_isFadingOut = true;
	m_isFadingIn = false;
	m_fadeOutSpeed = time;
}

void Camera::FadeIn(float time){
	m_isFadingOut = false;
	m_isFadingIn = true;
	m_fadeInSpeed = time;
}

void Camera::SetSmooth(bool smooth){
	m_smoothMovement = smooth;
}


void Camera::Shake(float duration, float intensity){

	m_shakingDuration = duration;
	m_shakingIntensity = intensity;
	m_shakMultiplicator = -1;
	m_shakeTimer = 0.f;
	m_initialOffset = m_offset.x;
}

bool Camera::ShakeIsEnded(){
	if (m_shakingDuration == 0) {
		return true;
	}
	return false;
}

