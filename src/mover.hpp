#ifndef MOVER_HPP
#define MOVER_HPP

#include <SFML/System.hpp>
#include "JsonBox.h"

#include <iostream>

class TileMap;

// Direction the entity is moving in
enum class Direction { NORTH = 'n', SOUTH = 's', EAST = 'e', WEST = 'w', NONE = 0 };
// Convert a cardinal direction n,e,s,w to a unit vector
sf::Vector2f dirToVec(Direction dir);
// Calculate the predominant cardinal direction a vector is pointing in
Direction vecToDir(const sf::Vector2f& dir);

class Mover
{
	protected:

	// Position of the entity in the game world, in grid squares not pixels
	sf::Vector2f pos;

	public:

	virtual Mover* clone() const = 0;

	virtual ~Mover() {}

	virtual const sf::Vector2f& getPosition() const { return pos; }
	virtual void setPosition(const sf::Vector2f pos) { this->pos = pos; }

	virtual bool isMoving() const = 0;
	virtual Direction getFacing() const = 0;
	virtual void setFacing(Direction facing) = 0;

	// Update the entity's position according to their velocity
	virtual void update(float dt) = 0;

	// Attempt to move the entity in the direction dir in the environment
	// described by tm over a time interval dt
	virtual void step(float dt, Direction dir, const TileMap& tm) = 0;

	virtual JsonBox::Object toJson() const
	{
		JsonBox::Object o;
		o["x"] = JsonBox::Value((int)pos.x);
		o["y"] = JsonBox::Value((int)pos.y);
		o["facing"] = JsonBox::Value(std::string(1, static_cast<char>(getFacing())));

		return o;
	}

	virtual void load(const JsonBox::Value& v)
	{
		JsonBox::Object o = v.getObject();
		setPosition(sf::Vector2f(o["x"].getInteger(), o["y"].getInteger()));
		setFacing(static_cast<Direction>(o["facing"].getString()[0]));
	}
};

#endif /* MOVER_HPP */
