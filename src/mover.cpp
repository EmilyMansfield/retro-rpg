#include <SFML/Graphics.hpp>
#include <cmath>

#include "mover.hpp"

sf::Vector2f dirToVec(Direction dir)
{
	switch(dir)
	{
		case Direction::NORTH:
		return sf::Vector2f(0, -1);

		case Direction::EAST:
		return sf::Vector2f(1, 0);

		case Direction::SOUTH:
		return sf::Vector2f(0, 1);

		case Direction::WEST:
		return sf::Vector2f(-1, 0);

		default:
		return sf::Vector2f(0, 0);
	}
}

Direction vecToDir(sf::Vector2f dir)
{
	if(fabs(dir.x) > fabs(dir.y))
		return (dir.x > 0 ? Direction::EAST : Direction::WEST);
	else
		return (dir.y > 0 ? Direction::SOUTH : Direction::NORTH);
}
