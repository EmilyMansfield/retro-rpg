#ifndef MOVEMENT_CONTROLLER_HPP
#define MOVEMENT_CONTROLLER_HPP

#include <SFML/System.hpp>

#include "tile_map.hpp"

// Direction the entity is moving in
enum class Direction { NORTH = 'n', SOUTH = 's', EAST = 'e', WEST = 'w', NONE = 0 };

class MovementController
{
	private:

	Direction moveIntention;
	Direction lastMove;
	sf::Vector2f destination;
	sf::Vector2f lastPos;
	float moveTimer;
	float moveDelay;
	bool moveTimerOn;

	sf::Vector2f dirToVec(Direction dir);
	Direction vecToDir(sf::Vector2f dir);
	
	void continueMovingTo();
	void continueMovingFrom();
	void changeDirection(Direction dir);
	void startMoving(Direction dir);
	void stopMoving();
	bool justReachedDestination();
	bool canMoveIn(sf::Vector2f start, Direction dir, TileMap& tm);

	public:

	// Position of the entity in the game world, in grid squares not pixels
	sf::Vector2f pos;
	// Current velocity the entity is moving with, in vector grid squares per second
	sf::Vector2f velocity;
	// Speed at which the entity moves, in grid squares per second
	float speed;
	bool moving;

	MovementController(float speed = 0.0f, float moveDelay = 0.0f)
	{
		this->moveIntention = Direction::NONE;
		this->lastMove = Direction::SOUTH;
		this->moving = false;
		this->moveTimer = 0.0f;
		this->moveTimerOn = false;
		this->speed = speed;
		this->moveDelay = moveDelay;
	}

	void setPos(sf::Vector2f pos);
	void update(float dt);
	void step(float dt, Direction dir, TileMap& tm);
	Direction movementDir();
};

#endif /* MOVEMENT_CONTROLLER_HPP */
