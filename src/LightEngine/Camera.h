#pragma once

#include "SFML/System.hpp"
#include "SFML/Window.hpp"

#include "Entity.h"

#include "Component.h"
#include "Updatable.h"
#include "Inspectable.h"


class Camera: public Component, public Updatable, public Inspectable{
	sf::View m_view;

	Entity* m_link;

	bool m_isLink;

	sf::Vector2f m_viewSize;
	sf::Vector2f m_offset;

	bool m_smoothMovement;
	bool m_hasMooved = false;
	float m_cameraXSpeed;
	sf::Vector2f previousLinkCoordinates{ 0, 0 };
	sf::Vector2f previousCoordinates{ 0, 0 };


	sf::Vector2f m_GoToPosition;
	sf::Vector2f m_GoToSize;
	float m_resizeSpeed;
	float m_moveSpeed;
	bool m_haveToMove;
	bool m_HaveToResize;

	bool m_followEntity;

	float m_fadeInSpeed = 0;
	float m_fadeOutSpeed = 0;
	bool m_isFadingIn = false;
	bool m_isFadingOut = false;

	bool m_isMainCam = false;



	float m_shakingDuration;
	float m_shakingIntensity;

	int m_shakMultiplicator;
	float m_shakeTimer;
	float m_initialOffset;


public:

	Camera();

	void TransitionTo(sf::FloatRect newView, float resizeSpeed, float movementSpeed);
	void TransitionTo(Entity* entity, sf::Vector2f newSize, float resizeSpeed, float movementSpeed);


	void SetLink(Entity* link);
	void Update() override;

	void SetMainCamera();

	void FixCamera();

	void SetViewSize(sf::Vector2f size);
	void SetOffset(sf::Vector2f offset);
	void Inspect() override;

	void Rotate(float rotation);

	void FadeOut(float time);
	void FadeIn(float time);
	void SetSmooth(bool smooth);

	void SetLink(bool value) { m_isLink = value; }


	bool IsTransitonMooving() { return m_haveToMove; }
	bool IsTransitionResizing() { return m_HaveToResize; }
	bool IsTransitionFinish() { return m_haveToMove && m_HaveToResize; }

	bool IsMainCam(){ return m_isMainCam; }

	void Shake(float duration, float intensity);
	bool ShakeIsEnded();

};
