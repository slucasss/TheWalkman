#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <cmath>
#include "Utils.h"

namespace Utils 
{
    bool Normalize(sf::Vector2f& vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;
		
			return true;
		}

		return false;
    }

	float Dot(const sf::Vector2f& vector1, const sf::Vector2f& vector2)
	{
		return vector1.x * vector2.x + vector1.y * vector2.y;
	}

	float GetDistance(int x1, int y1, int x2, int y2)
	{
		int x = x2 - x1;
		int y = y2 - y1;

		return std::sqrt(x * x + y * y);
	}

	float GetDistance(const sf::Vector2f & pos1, const sf::Vector2f& pos2)
	{
		return GetDistance(pos1.x, pos1.y, pos2.x, pos2.y);
	}

	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		float dot = v1.x * v2.x + v1.y * v2.y;
		float det = v1.x * v2.y - v1.y * v2.x;

		return std::atan2(det, dot) * 180 / 3.14159265;
	}

}