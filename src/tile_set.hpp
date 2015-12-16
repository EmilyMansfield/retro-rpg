#ifndef TILE_SET_HPP
#define TILE_SET_HPP

#include <string>
#include <JsonBox.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "entity.hpp"

class EntityManager;

class Animation
{
	public:

	unsigned int x;
	unsigned int y;
	unsigned int len;

	Animation() {}
	Animation(unsigned int x, unsigned int y, unsigned int len)
	{
		this->x = x;
		this->y = y;
		this->len = len;		
	}

};

class TileSet : public Entity
{
	public:

	sf::Texture tex;
	unsigned int tilesize;

	// Map of animations stored in the sprite sheet, each labelled by
	// a string id
	std::map<std::string, Animation> animations;

	// Constructors
	TileSet(const std::string& id,
			const std::string& filename,
			unsigned int tilesize);
	TileSet(const std::string& id,
			const JsonBox::Value& v,
			EntityManager* mgr);

	// Load the tile set from a json file
	// JSON is not really very useful for this, but its a requirement of
	// the EntityManager so we have to implement it
	virtual void load(const JsonBox::Value& v, EntityManager* mgr);
};

#endif /* TILE_SET_HPP */
