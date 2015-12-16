#ifndef AREA_HPP
#define AREA_HPP

#include <vector>
#include <string>
#include <JsonBox.h>

#include "entity.hpp"
#include "inventory.hpp"
#include "creature.hpp"
#include "dialogue.hpp"
#include "tile_set.hpp"
#include "tile_map.hpp"

class EntityManager;
class Door;
class TreasureChest;

// Movement is achieved through the use of areas, which are contained
// units of space consisting of an inventory, a list of creatures and
// a dialogue
class Area : public Entity
{
	public:

	// Dialogue is run whenever the area is entered
	Dialogue dialogue;

	// Links between rooms. Every door should have this as one of its area
	// pointers
	std::vector<Door*> doors;

	// Creatures contained within the area. Not pointers because we want unique
	// instances of the creatures
	std::vector<Creature> creatures;

	// Treasure chests in the rooms, each with an inventory
	std::vector<TreasureChest> chests;

	// Tileset used to draw the area
	TileSet* tileset;

	// Tilemap describing the appearance of the area
	TileMap tilemap;

	// Constructor
	Area(const std::string& id, const JsonBox::Value& v, EntityManager* mgr);

	// Load the area from the given Json value
	void load(const JsonBox::Value& v, EntityManager* mgr);

	// Return a Json object representing the area
	JsonBox::Object getJson() const;
};

#endif /* AREA_HPP */
