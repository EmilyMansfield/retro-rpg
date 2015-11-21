#ifndef MOVEMENT_CONTROLLER_HPP
#define MOVEMENT_CONTROLLER_HPP

#include <SFML/System.hpp>

#include "tile_map.hpp"

// Direction the entity is moving in
enum class Direction { NORTH = 'n', SOUTH = 's', EAST = 'e', WEST = 'w', NONE = 0 };

class MovementController
{
	private:

	// Direction the entity will move in when its current
	// movement has been completed
	Direction moveIntention;
	// Direction the entity moved in before its current movement
	Direction lastMove;
	// Position the entity is moving to, will differ from the
	// position the entity started from by a unit vector n,s,e,w
	sf::Vector2f destination;
	// Position the entity was at in the previous frame
	sf::Vector2f lastPos;

	// When entity is stationary and controller by the player, movement
	// only begins after a short moveDelay so that the player can change
	// direction without moving
	float moveTimer;
	float moveDelay;
	bool moveTimerOn;

	// Convert a cardinal direction n,e,s,w to a unit vector
	sf::Vector2f dirToVec(Direction dir);
	// Calculate the predominant cardinal direction a vector is pointing in
	Direction vecToDir(sf::Vector2f dir);

	// Calculate the velocity vector from the entity's current position
	// to its desination
	void setVelToDest();
	// Move the destination to the next adjacent tile and recalculate
	// the velocity vector
	void resetVelToDest();
	// Change movement direction
	void changeDirection(Direction dir);
	// Begin moving from stationary
	void startMoving(Direction dir);
	// Snap to destination and stop moving
	void stopMoving();
	// True if the entity reached their destination between the last
	// frame and the current frame
	bool justReachedDestination();
	// True if the tile in the direction dir from the position start is unblocked
	bool canMoveIn(sf::Vector2f start, Direction dir, TileMap& tm);

	public:

	// Position of the entity in the game world, in grid squares not pixels
	sf::Vector2f pos;
	// Current velocity the entity is moving with, in vector grid squares per second
	sf::Vector2f velocity;
	// Speed at which the entity moves, in grid squares per second
	// The magnitude of the velocity vector should equal the speed
	float speed;
	// True if velocity is non-zero
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

	// Set the position of the entity
	void setPos(sf::Vector2f pos);
	// Update the entity's position according to their velocity
	void update(float dt);
	// Attempt to move the entity in the direction dir in the environment
	// described by tm over a time interval dt
	void step(float dt, Direction dir, TileMap& tm);
	// Direction the entity is facing, equal to the direction of
	// the last non-zero velocity the entity had
	Direction movementDir();
};

#endif /* MOVEMENT_CONTROLLER_HPP */
