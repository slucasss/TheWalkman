#pragma once
#include <vector>

#include "SFML/System.hpp"
#include "Entity.h"


class AmbiantShader{
	static AmbiantShader* Instance;


	struct Light {
		sf::Vector2f WorldPos;
		float radius;
		float intensity;

		float r;
		float g;
		float b;
	};
	struct ConeLight {
		sf::Vector2f WorldPos;
		float height;
		float intensity;

		float r;
		float g;
		float b;

		float dirX;
		float dirY;
		float halfAngle;
	};

	struct Player {
		Entity* entity;
		float radius;
		float intensity;

		float r;
		float g;
		float b;
	};

	std::vector<Light> m_lights;
	int m_lightId = 0;

	std::vector<ConeLight> m_coneLights;
	int m_coneLightId = 0;

	Player m_player;
	


public:
	static AmbiantShader* Get() {
		if (Instance == nullptr) {
			Instance = new AmbiantShader;
		}
		return Instance;
	}

	void Update();

	//Position, rayon du cercle de lumire, intensite(1.0 = lumière moyenne), color(rgb/255)
	int AddLight(sf::Vector2f pos, float radius, float intensity, sf::Vector3f color);

	//Position, hauteur du cone, intensité ( 1.0 = lumière moyenne), color (rgb/255), direction en vecteur normalisé, angle de l'ouverture a la base du cone en radians
	int AddConeLight(sf::Vector2f pos, float height, float intensity, sf::Vector3f color, sf::Vector2f dir, float halfAngle);

	void AddPlayer(Entity* entity, float radius, float intensity, sf::Vector3f color);
	void SetDrunkFactor(float factor);
	void Pulse(float strenght);

	void SetLightIntensity(float intensity, int id);
	void SetConeLightIntensity(float intensity, int id);
	void SetPlayerIntensity(float intensity);
	;
};

