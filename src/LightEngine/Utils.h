#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace Utils
{
	bool Normalize(sf::Vector2f& vector);
	float Dot(const sf::Vector2f & vector1, const sf::Vector2f & vector2);
	float GetDistance(int x1, int y1, int x2, int y2);
	float GetDistance(const sf::Vector2f & pos1, const sf::Vector2f& pos2);
	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2);

	bool IsInsideRectangle(sf::FloatRect rect, sf::Vector2f pos);
}