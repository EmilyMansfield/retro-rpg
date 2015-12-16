#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <unordered_set>
#include <string>
#include <SFML/System.hpp>
#include <JsonBox.h>

#include "creature.hpp"

class EntityManager;

class Player : public Creature
{
	public:

	// Level of the player
	unsigned int level;

	// Name of the player's class
	// Class may be Fighter, Rogue etc
	std::string className;

	// Ids of areas visited by the player
	std::unordered_set<std::string> visitedAreas;

	// Constructors
	Player(const std::string& name,
		   int hp,
		   int strength,
		   int agility,
		   double evasion,
		   unsigned int xp,
		   unsigned int level,
		   const std::string& className);
	Player();
	Player(const JsonBox::Value& saveData,
		   const JsonBox::Value& areaData,
		   EntityManager* mgr);

	// Calculates the total experience required to reach a certain level
	unsigned int xpToLevel(unsigned int level) const;

	// Level the player to the next level if it has enough experience
	// to do so, returning true if it could level up and false otherwise.
	bool levelUp();

	// Create a Json object representation of the player
	JsonBox::Object toJson() const;

	// Save the player to a file named after them
	void save(EntityManager* mgr) const;

	// Attempt to load all data from the JSON value
	void load(const JsonBox::Value& saveData, EntityManager* mgr);
	void loadArea(const JsonBox::Value& areaData, EntityManager* mgr);
};

#endif /* PLAYER_HPP */
