#ifndef STATIC_MOVER_HPP
#define STATIC_MOVER_HPP

#include "mover.hpp"

class TileMap;

// Describes a permanently stationary object such as a treasure chest
// or statue, that aren't part of the terrain but should never move
class StaticMover : public Mover
{
	private:

	Direction facing;

	public:

	StaticMover(Direction facing) : facing(facing) {}

	StaticMover* clone() const { return new StaticMover(*this); }

	bool isMoving() const { return false; }
	Direction getFacing() const { return facing; }

	void update(float dt) {}
	void step(float dt, Direction dir, const TileMap& tm) {}
};

#endif /* STATIC_MOVER_HPP */
