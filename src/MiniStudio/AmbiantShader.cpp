#include "AmbiantShader.h"

#include "Managers/ShaderManager.h"
#include "GameManager.h"
#include "TransformComponent.h"

AmbiantShader* AmbiantShader::Instance = nullptr;

void AmbiantShader::Update(){
	ShaderManager* manager = ShaderManager::Get();
	std::string path = "../../../src/Shaders/AmbiantShader.frag";
	
	
	manager->SetParameter<int>(path, "lightCount", (int)m_lights.size());

	manager->SetParameter<int>(path, "coneCount", (int)m_coneLights.size());

	int i = 0;
	for (Light& l : m_lights) {
		sf::Vector2i posI = GameManager::Get()->GetSceneTexture()->mapCoordsToPixel(l.WorldPos);
		sf::Vector2f pos{ (float)posI.x, (float)posI.y };

		manager->SetParameter<sf::Glsl::Vec4>(path,("lights[" + std::to_string(i) + "]").c_str(),sf::Glsl::Vec4(pos.x, pos.y, l.radius, l.intensity));

		manager->SetParameter<sf::Glsl::Vec3>(path,("lightsColor[" + std::to_string(i) + "]").c_str(),sf::Glsl::Vec3(l.r, l.g, l.b));
		i++;
	}


	i = 0;
	for (ConeLight& l : m_coneLights) {
		sf::Vector2f worldpos{ GameManager::Get()->GetSceneTexture()->mapCoordsToPixel(l.WorldPos) };
		manager->SetParameter<sf::Glsl::Vec4>(path, ("cones[" + std::to_string(i) + "]").c_str(), sf::Glsl::Vec4(worldpos.x, worldpos.y, l.height, l.intensity));

		manager->SetParameter<sf::Glsl::Vec3>(path, ("conesColor[" + std::to_string(i) + "]").c_str(), sf::Glsl::Vec3(l.r, l.g, l.b));

		manager->SetParameter<sf::Glsl::Vec4>(path, ("conesDirs[" + std::to_string(i) + "]").c_str(), sf::Glsl::Vec4(l.dirX, l.dirY, l.halfAngle, 0.0));
		i++;
	}

	if(m_player.entity != nullptr){

		sf::Vector2i posI = GameManager::Get()->GetSceneTexture()->mapCoordsToPixel(m_player.entity->GetComponent<TransformComponent>()->GetPosition());
		sf::Vector2f pos{ (float)posI.x, (float)posI.y };

		manager->SetParameter<sf::Glsl::Vec4>(path, "player", sf::Glsl::Vec4(pos.x, pos.y, m_player.radius, m_player.intensity));

		manager->SetParameter<sf::Glsl::Vec3>(path, "playerColor", sf::Glsl::Vec3(m_player.r, m_player.g, m_player.b));
	}
}

int AmbiantShader::AddLight(sf::Vector2f pos, float radius, float intensity, sf::Vector3f color){
	Light light{ pos, radius, intensity, color.x, color.y, color.z };

	m_lights.push_back(light);

	ShaderManager* manager = ShaderManager::Get();
	std::string path = "../../../src/Shaders/AmbiantShader.frag";

	sf::Vector2f worldpos{ GameManager::Get()->GetSceneTexture()->mapCoordsToPixel(light.WorldPos) };
	manager->SetParameter<sf::Glsl::Vec4>(path, ("lights[" + std::to_string(m_lightId) + "]").c_str(), sf::Glsl::Vec4(worldpos.x, worldpos.y, light.radius, light.intensity));

	manager->SetParameter<sf::Glsl::Vec3>(path, ("lightsColor[" + std::to_string(m_lightId) + "]").c_str(), sf::Glsl::Vec3(light.r,light.g, light.b ));

	manager->SetParameter<int>(path, "lightCount", m_lights.size());
	m_lightId += 1;
	return m_lightId-1;
}

int AmbiantShader::AddConeLight(sf::Vector2f pos, float height, float intensity, sf::Vector3f color, sf::Vector2f dir, float halfAngle){
	ConeLight conelight{ pos, height, intensity, color.x, color.y, color.z , dir.x, dir.y, halfAngle };

	m_coneLights.push_back(conelight);

	ShaderManager* manager = ShaderManager::Get();
	std::string path = "../../../src/Shaders/AmbiantShader.frag";



	sf::Vector2f worldpos{ GameManager::Get()->GetSceneTexture()->mapCoordsToPixel(conelight.WorldPos) };
	manager->SetParameter<sf::Glsl::Vec4>(path, ("cones[" + std::to_string(m_coneLightId) + "]").c_str(), sf::Glsl::Vec4(worldpos.x, worldpos.y, height, intensity));

	manager->SetParameter<sf::Glsl::Vec3>(path, ("conesColor[" + std::to_string(m_coneLightId) + "]").c_str(), sf::Glsl::Vec3(conelight.r, conelight.g, conelight.b));

	manager->SetParameter<sf::Glsl::Vec4>(path, ("conesDirs[" + std::to_string(m_coneLightId) + "]").c_str(), sf::Glsl::Vec4(dir.x, dir.y, halfAngle, 0.0));

	manager->SetParameter<int>(path, "coneCount", m_coneLights.size());
	m_coneLightId += 1;
	return m_coneLightId -1;
}

void AmbiantShader::AddPlayer(Entity* entity, float radius, float intensity, sf::Vector3f color){

	TransformComponent* transform = entity->GetComponent<TransformComponent>();
	Player light{ entity, radius, intensity, color.x, color.y, color.z};
	m_player = light;

	ShaderManager* manager = ShaderManager::Get();
	std::string path = "../../../src/Shaders/AmbiantShader.frag";


	sf::Vector2f worldpos{ GameManager::Get()->GetSceneTexture()->mapCoordsToPixel(transform->GetPosition()) };
	manager->SetParameter<sf::Glsl::Vec4>(path, "player", sf::Glsl::Vec4(worldpos.x, worldpos.y, m_player.radius, m_player.intensity));
	manager->SetParameter<sf::Glsl::Vec3>(path, "playerColor", sf::Glsl::Vec3(m_player.r, m_player.g, m_player.b));
}


void AmbiantShader::SetDrunkFactor(float factor) {
	std::string path = "../../../src/Shaders/AmbiantShader.frag";
	ShaderManager::Get()->SetParameter<float>(path, "drunkStrength" , factor);
}


void AmbiantShader::Pulse(float strength) {
	ShaderManager* manager = ShaderManager::Get();
	std::string path = "../../../src/Shaders/AmbiantShader.frag";

	manager->SetParameter<float>(path, "beatTime", GameManager::Get()->GetShaderClock().getElapsedTime().asSeconds());
	manager->SetParameter<float>(path, "waveStrength", strength);
}



void AmbiantShader::SetLightIntensity(float intensity, int id){
	m_lights[id].intensity = intensity;
}

void AmbiantShader::SetConeLightIntensity(float intensity, int id){
	m_coneLights[id].intensity = intensity;
}

void AmbiantShader::SetPlayerIntensity(float intensity){
	m_player.intensity = intensity;
}
