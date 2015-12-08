#include <SFML/System.hpp>
#include <cmath>

#include "creature_mover.hpp"
#include "tile_map.hpp"

void CreatureMover::setVelToDest()
{
	velocity = destination - pos;
	velocity /= (float)sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
	velocity *= speed;
}

void CreatureMover::resetVelToDest()
{
	// Set new destination
	destination = destination + dirToVec(lastMove);
	// Move towards new destination
	setVelToDest();
}

void CreatureMover::changeDirection(Direction dir)
{
	// Snap to destination tile
	setPosition(destination);
	// Set new destination
	destination = destination + dirToVec(dir);
	// Move towards the new destination
	setVelToDest();
	// Remember move
	lastMove = dir;
}

void CreatureMover::startMoving(Direction dir)
{
	// Set new destination
	destination = pos + dirToVec(dir);
	// Move
	moving = true;
	setVelToDest();
	// Remember move
	lastMove = dir;
}

void CreatureMover::stopMoving()
{
	// Snap to destination tile to avoid inaccuracies
	setPosition(destination);
	// Stop moving
	moving = false;
	velocity.x = 0.0f;
	velocity.y = 0.0f;
}

bool CreatureMover::justReachedDestination()
{
	return (pos.x >= destination.x && lastPos.x < destination.x) ||
		(pos.x <= destination.x && lastPos.x > destination.x) ||
		(pos.y >= destination.y && lastPos.y < destination.y) ||
		(pos.y <= destination.y && lastPos.y > destination.y);
}

bool CreatureMover::canMoveIn(const sf::Vector2f& start, Direction dir, const TileMap& tm)
{
	return tm.at(start + dirToVec(dir)) == 0;
}

void CreatureMover::update(float dt)
{
	// Move entity according to velocity
	lastPos = pos;
	pos += dt * velocity;
}

void CreatureMover::step(float dt, Direction dir, const TileMap& tm)
{
	// Set direction to move in
	moveIntention = dir;

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
	else if(!moveTimerOn && !moving && moveIntention != Direction::NONE &&
		canMoveIn(pos, moveIntention, tm))
	{
		moveTimerOn = true;
		moveTimer = 0.0f;
		lastMove = moveIntention;
	}

	// Start moving from stationary
	else if(moveTimerOn && moveTimer >= moveDelay && !moving && moveIntention != Direction::NONE &&
		canMoveIn(pos, moveIntention, tm))
	{
		startMoving(moveIntention);
		moveTimerOn = false;
		moveTimer = 0.0f;
	}

	// Change direction but don't move
	else if(!moving && moveIntention != Direction::NONE &&
		!canMoveIn(pos, moveIntention, tm))
	{
		lastMove = moveIntention;
	}
}

bool CreatureMover::isMoving() const
{
	return moving;
}

Direction CreatureMover::getFacing() const
{
	if(velocity == sf::Vector2f(0,0)) return lastMove;
	else return vecToDir(velocity);
}

void CreatureMover::setFacing(Direction facing)
{
	lastMove = facing;
}
