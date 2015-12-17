#ifndef TREASURE_CHEST_HPP
#define TREASURE_CHEST_HPP

#include <string>

#include "activator.hpp"
#include "inventory.hpp"

class Creature;
class TileSet;

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

	TreasureChest(const Inventory& inventory,
				  Direction facing,
				  bool open,
				  float speed,
				  TileSet* tiles);

	TreasureChest(TreasureChest&& rhs);

	void toggle(Creature& user);
	void set(bool on);

	void update(float dt);

	// Note that getOpen != !getClosed
	bool getOpen() const;
	bool getClosed() const;
};

#endif /* TREASURE_CHEST_HPP */
