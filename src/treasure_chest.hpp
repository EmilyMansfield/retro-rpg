#ifndef TREASURE_CHEST_HPP
#define TREASURE_CHEST_HPP

#include <string>

#include "mover.hpp"
#include "renderer.hpp"
#include "overworldable.hpp"
#include "static_mover.hpp"
#include "entity_renderer.hpp"
#include "activator.hpp"

class Creature;

class TreasureChest : public Activator
{
	private:

	enum class State { CLOSED, OPENING, OPEN, CLOSING };
	State state;

	// Animation name and interpolation capped between 0 and 1
	float interp;
	std::string animStr;

	// Speed at which the treasure chest opens, in fraction of
	// animation per second
	float speed;

	public:

	Inventory inventory;

	void toggle(const Creature& user)
	{
		if(state == State::CLOSED || state == State::CLOSING)
			state = State::OPENING;
		else
			state = State::CLOSING;
		interp = 0.0f;
		animStr = std::string("chest_opening_") + static_cast<char>(mover->getFacing());
	}

	void set(const Creature& user, bool on)
	{
		state = (on ? State::OPENING : State::CLOSING);
		interp = 0.0f;
		animStr = std::string("chest_opening_") + static_cast<char>(mover->getFacing());
	}

	void update(float dt)
	{
		if(state == State::CLOSED) interp = 1.0f;
		else if(state == State::OPEN) interp = 0.0f;
		else
		{
			if(interp < 1.0f) interp += speed * dt;
			if(interp > 1.0f) interp = 1.0f;
		}
		if(state == State::CLOSING || state == State::CLOSED)
			renderer->setFrame(animStr, 1.0f-interp);
		else
			renderer->setFrame(animStr, interp);
	}

	TreasureChest(const Inventory& inventory, Direction facing,
		bool open, float speed, TileSet* tiles) :
		inventory(inventory), speed(speed)
	{
		attachMover<StaticMover>(facing);
		attachRenderer<EntityRenderer>(tiles);
		state = (open ? State::OPEN : State::CLOSED);
		animStr = std::string("chest_opening_") + static_cast<char>(mover->getFacing());
		interp = 0.0f;
	}

	TreasureChest(TreasureChest&& rhs) :
		state(std::move(rhs.state)),
		interp(std::move(rhs.interp)),
		animStr(std::move(rhs.animStr)),
		speed(std::move(rhs.speed)),
		inventory(std::move(rhs.inventory))
	{
		mover.reset(rhs.mover.release());
		renderer.reset(rhs.renderer.release());
	}
};

#endif /* TREASURE_CHEST_HPP */
