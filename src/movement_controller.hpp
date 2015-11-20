#ifndef MOVEMENT_CONTROLLER_HPP
#define MOVEMENT_CONTROLLER_HPP

#include "tile_set.hpp"
#include "tile_map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <cmath>

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

	void continueMovingTo()
	{
		velocity = destination - pos;
		velocity /= (float)sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
		velocity *= speed;
	}

	void continueMovingFrom()
	{
		// Set new destination
		destination = destination + dirToVec(lastMove);
		// Move towards new destination
		continueMovingTo();
	}

	void changeDirection(Direction dir)
	{
		// Snap to destination tile
		setPos(destination);
		// Set new destination
		destination = destination + dirToVec(dir);
		// Move towards the new destination
		continueMovingTo();
		// Remember move
		lastMove = dir;
	}

	void startMoving(Direction dir)
	{
		// Set new destination
		destination = pos + dirToVec(dir);
		// Move
		moving = true;
		continueMovingTo();
		// Remember move
		lastMove = dir;
	}

	void stopMoving()
	{
		// Snap to destination tile to avoid inaccuracies
		setPos(destination);
		// Stop moving
		moving = false;
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}

	bool justReachedDestination()
	{
		return (pos.x >= destination.x && lastPos.x < destination.x) ||
			(pos.x <= destination.x && lastPos.x > destination.x) ||
			(pos.y >= destination.y && lastPos.y < destination.y) ||
			(pos.y <= destination.y && lastPos.y > destination.y);
	}

	bool canMoveIn(sf::Vector2f start, Direction dir, TileMap& tm)
	{
		return tm.at(start + dirToVec(dir)) == 0;
	}

	public:

	// Position of the entity in the game world, in grid squares not pixels
	sf::Vector2f pos;
	// Current velocity the entity is moving with, in vector grid squares per second
	sf::Vector2f velocity;
	// Speed at which the entity moves, in grid squares per second
	float speed;
	bool moving;

	void setPos(sf::Vector2f pos)
	{
		this->pos = pos;
	}

	MovementController(float speed=0.0f, float moveDelay=0.0f)
	{
		this->moveIntention = Direction::NONE;
		this->lastMove = Direction::SOUTH;
		this->moving = false;
		this->moveTimer = 0.0f;
		this->moveTimerOn = false;
		this->speed = speed;
		this->moveDelay = moveDelay;
	}

	void update(float dt)
	{
		// Move entity according to velocity
		lastPos = pos;
		setPos(pos + dt * velocity);
	}

	void step(float dt, Direction dir, TileMap& tm)
	{
		// Set direction to move in
		this->moveIntention = dir;

		// Increment the move delay counter if necessary
		if(moveTimerOn) moveTimer += dt;
		// Reset timer if no move button is held
		if(moveIntention == Direction::NONE)
		{
			moveTimerOn = false;
			moveTimer = 0.0f;
		}

		// Stop if at the destination
		if(moving && justReachedDestination() && moveIntention == Direction::NONE)
		{
			stopMoving();
		}

		// Stop if at a wall
		else if(moving && justReachedDestination() && moveIntention != Direction::NONE &&
			!canMoveIn(destination, moveIntention, tm))
		{
			stopMoving();
		}

		// Destination reached but continue in the same direction
		else if(moving && justReachedDestination() && moveIntention != Direction::NONE &&
			canMoveIn(destination, moveIntention, tm) && moveIntention == lastMove)
		{
			continueMovingFrom();
		}

		// Destination reached but continue in a different direction
		else if(moving && justReachedDestination() && moveIntention != Direction::NONE &&
			canMoveIn(destination, moveIntention, tm) && moveIntention != lastMove)
		{
			changeDirection(moveIntention);
		}

		// Destination not reached so keep going
		else if(moving && !justReachedDestination())
		{
			continueMovingTo();
		}

		// Change direction from stationary, and start movement timer
		else if(!moveTimerOn && !moving && moveIntention != Direction::NONE && canMoveIn(pos, moveIntention, tm))
		{
			moveTimerOn = true;
			moveTimer = 0.0f;
			lastMove = moveIntention;
		}

		// Start moving from stationary
		else if(moveTimerOn && moveTimer >= moveDelay && !moving && moveIntention != Direction::NONE && canMoveIn(pos, moveIntention, tm))
		{
			startMoving(moveIntention);
			moveTimerOn = false;
			moveTimer = 0.0f;
		}

		// Change direction but don't move
		else if(!moving && moveIntention != Direction::NONE && !canMoveIn(pos, moveIntention, tm))
		{
			lastMove = moveIntention;
		}
	}

	Direction movementDir()
	{
		if(velocity == sf::Vector2f(0,0)) return lastMove;
		else return vecToDir(velocity);
	}
};

#endif /* MOVEMENT_CONTROLLER_HPP */
