#include "static_mover.hpp"

StaticMover::StaticMover(Direction facing) : 
	facing(facing) {}

StaticMover* StaticMover::clone() const
{
	return new StaticMover(*this);
}

bool StaticMover::isMoving() const
{
	return false;
}
Direction StaticMover::getFacing() const
{
	return facing;
}
void StaticMover::setFacing(Direction facing)
{
	this->facing = facing;
}

void StaticMover::StaticMover::update(float dt)
{

}

void StaticMover::step(float dt, Direction dir, const TileMap& tm)
{

}
