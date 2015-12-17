#include <string>

#include "mover.hpp"
#include "renderer.hpp"
#include "overworldable.hpp"
#include "static_mover.hpp"
#include "entity_renderer.hpp"
#include "treasure_chest.hpp"

TreasureChest::TreasureChest(const Inventory& inventory,
			  Direction facing,
			  bool open,
			  float speed,
			  TileSet* tiles) :
	speed(speed),
	inventory(inventory)
{
	attachMover<StaticMover>(facing);
	attachRenderer<EntityRenderer>(tiles);
	state = (open ? State::OPEN : State::CLOSED);
	animStr = std::string("chest_opening_") + static_cast<char>(mover->getFacing());
	update(0);
}

TreasureChest::TreasureChest(TreasureChest&& rhs) :
	state(std::move(rhs.state)),
	interp(std::move(rhs.interp)),
	animStr(std::move(rhs.animStr)),
	speed(std::move(rhs.speed)),
	inventory(std::move(rhs.inventory))
{
	mover.reset(rhs.mover.release());
	renderer.reset(rhs.renderer.release());
}

void TreasureChest::toggle(Creature& user)
{
	if(state == State::CLOSED || state == State::CLOSING)
	{
		state = State::OPENING;
		user.inventory += inventory;
		inventory.clear();
	}
	else
	{
		state = State::CLOSING;
	}
	interp = 0.0f;
	animStr = std::string("chest_opening_") + static_cast<char>(mover->getFacing());
}

void TreasureChest::set(bool on)
{
	state = (on ? State::OPENING : State::CLOSING);
	interp = 0.0f;
	animStr = std::string("chest_opening_") + static_cast<char>(mover->getFacing());
}

void TreasureChest::update(float dt)
{
	if(state == State::OPEN || state == State::CLOSED)
		interp = 1.0f;
	else
	{
		if(interp < 1.0f) interp += speed * dt;
		if(interp > 1.0f)
		{
			interp = 1.0f;
			if(state == State::OPENING) state = State::OPEN;
			else if(state == State::CLOSING) state = State::CLOSED;
		}
	}
	if(state == State::CLOSING || state == State::CLOSED)
		renderer->setFrame(animStr, 1.0f-interp);
	else
		renderer->setFrame(animStr, interp);
}

// Note that getOpen != !getClosed
bool TreasureChest::getOpen() const
{
	return state == State::OPEN;
}
bool TreasureChest::getClosed() const
{
	return state == State::CLOSED;
}
