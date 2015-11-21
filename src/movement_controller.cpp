#include <SFML/System.hpp>
#include <cmath>

#include "movement_controller.hpp"
#include "tile_set.hpp"
#include "tile_map.hpp"

sf::Vector2f MovementController::dirToVec(Direction dir)
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

Direction MovementController::vecToDir(sf::Vector2f dir)
{
	if(fabs(dir.x) > fabs(dir.y))
		return (dir.x > 0 ? Direction::EAST : Direction::WEST);
	else
		return (dir.y > 0 ? Direction::SOUTH : Direction::NORTH);
}

void MovementController::setVelToDest()
{
	velocity = destination - pos;
	velocity /= (float)sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	velocity *= speed;
}

void MovementController::resetVelToDest()
{
	// Set new destination
	destination = destination + dirToVec(lastMove);
	// Move towards new destination
	setVelToDest();
}

void MovementController::changeDirection(Direction dir)
{
	// Snap to destination tile
	setPos(destination);
	// Set new destination
	destination = destination + dirToVec(dir);
	// Move towards the new destination
	setVelToDest();
	// Remember move
	lastMove = dir;
}

void MovementController::startMoving(Direction dir)
{
	// Set new destination
	destination = pos + dirToVec(dir);
	// Move
	moving = true;
	setVelToDest();
	// Remember move
	lastMove = dir;
}

void MovementController::stopMoving()
{
	// Snap to destination tile to avoid inaccuracies
	setPos(destination);
	// Stop moving
	moving = false;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

bool MovementController::justReachedDestination()
{
	return (pos.x >= destination.x && lastPos.x < destination.x) ||
		(pos.x <= destination.x && lastPos.x > destination.x) ||
		(pos.y >= destination.y && lastPos.y < destination.y) ||
		(pos.y <= destination.y && lastPos.y > destination.y);
}

bool MovementController::canMoveIn(sf::Vector2f start, Direction dir, TileMap& tm)
{
	return tm.at(start + dirToVec(dir)) == 0;
}


void MovementController::setPos(sf::Vector2f pos)
{
	this->pos = pos;
}


void MovementController::update(float dt)
{
	// Move entity according to velocity
	lastPos = pos;
	setPos(pos + dt * velocity);
}

void MovementController::step(float dt, Direction dir, TileMap& tm)
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
		resetVelToDest();
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
		setVelToDest();
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

Direction MovementController::movementDir()
{
	if(velocity == sf::Vector2f(0,0)) return lastMove;
	else return vecToDir(velocity);
}
