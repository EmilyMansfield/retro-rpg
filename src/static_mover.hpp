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

	StaticMover(Direction facing);

	StaticMover* clone() const;

	bool isMoving() const;
	Direction getFacing() const;
	void setFacing(Direction facing);

	void update(float dt);
	void step(float dt, Direction dir, const TileMap& tm);
};

#endif /* STATIC_MOVER_HPP */
