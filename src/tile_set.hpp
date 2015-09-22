#ifndef TILE_SET_HPP
#define TILE_SET_HPP

#include <string>
#include "JsonBox.h"
#include <SFML/Graphics.hpp>

#include "entity.hpp"

class EntityManager;

class TileSet : public Entity
{
	public:

	sf::Texture tex;
	unsigned int tilesize;

	// Constructors
	TileSet(std::string id, std::string filename, unsigned int tilesize);
	TileSet(std::string id, JsonBox::Value& v, EntityManager* mgr);

	// Load the tile set from a json file
	// JSON is not really very useful for this, but its a requirement of
	// the EntityManager so we have to implement it
	virtual void load(JsonBox::Value& v, EntityManager* mgr);
};

#endif /* TILE_SET_HPP */
