#include <SFML/Graphics.hpp>
#include <cmath>

#include "mover.hpp"

Mover::~Mover() {}

const sf::Vector2f& Mover::getPosition() const
{
	return pos;
}
void Mover::setPosition(const sf::Vector2f pos)
{
	this->pos = pos;
}

JsonBox::Object Mover::toJson() const
{
	JsonBox::Object o;
	o["x"] = JsonBox::Value((int)pos.x);
	o["y"] = JsonBox::Value((int)pos.y);
	o["facing"] = JsonBox::Value(std::string(1, static_cast<char>(getFacing())));

	return o;
}

void Mover::load(const JsonBox::Value& v)
{
	JsonBox::Object o = v.getObject();
	setPosition(sf::Vector2f(o["x"].getInteger(), o["y"].getInteger()));
	setFacing(static_cast<Direction>(o["facing"].getString()[0]));
}

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

Direction vecToDir(const sf::Vector2f& dir)
{
	if(fabs(dir.x) > fabs(dir.y))
		return (dir.x > 0 ? Direction::EAST : Direction::WEST);
	else
		return (dir.y > 0 ? Direction::SOUTH : Direction::NORTH);
}
