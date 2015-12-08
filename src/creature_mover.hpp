#ifndef CREATURE_MOVER_HPP
#define CREATURE_MOVER_HPP

#include <SFML/System.hpp>

#include "mover.hpp"

class TileMap;

class CreatureMover : public Mover
{
	private:

	// Direction the creature will move in when its current
	// movement has been completed
	Direction moveIntention;
	// Direction the creature moved in before its current movement
	Direction lastMove;
	// Position the creature is moving to, will differ from the
	// position the creature started from by a unit vector n,s,e,w
	sf::Vector2f destination;
	// Position the creature was at in the previous frame
	sf::Vector2f lastPos;
	// Current velocity the creature is moving with, in vector grid squares per second
	sf::Vector2f velocity;
	// Speed at which the entity moves, in grid squares per second
	// The magnitude of the velocity vector should equal the speed
	float speed;
	// True if velocity is non-zero
	bool moving;

	// When creature is stationary and controller by the player, movement
	// only begins after a short moveDelay so that the player can change
	// direction without moving
	float moveTimer;
	float moveDelay;
	bool moveTimerOn;

	// Calculate the velocity vector from the creature's current position
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
	// True if the creature reached their destination between the last
	// frame and the current frame
	bool justReachedDestination();
	// True if the tile in the direction dir from the position start is unblocked
	bool canMoveIn(const sf::Vector2f& start, Direction dir, const TileMap& tm);

	public:

	CreatureMover(float speed = 0.0f,
				  float moveDelay = 0.0f) :
		moveIntention(Direction::NONE),
		lastMove(Direction::SOUTH),
		speed(speed),
		moving(false),
		moveTimer(0.0f),
		moveDelay(moveDelay),
		moveTimerOn(false) {}

	CreatureMover* clone() const { return new CreatureMover(*this); }

	bool isMoving() const;
	Direction getFacing() const;
	void setFacing(Direction facing);

	// Update the creature's position according to their velocity
	void update(float dt);

	// Attempt to move the creature in the direction dir in the environment
	// described by tm over a time interval dt
	void step(float dt, Direction dir, const TileMap& tm);
};

#endif /* CREATURE_MOVER_HPP */
