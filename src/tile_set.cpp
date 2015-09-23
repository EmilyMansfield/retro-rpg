#include <string>
#include "JsonBox.h"
#include <SFML/Graphics.hpp>

#include "tile_set.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"

TileSet::TileSet(std::string id, std::string filename,
	unsigned int tilesize) : Entity(id)
{
	this->tex.loadFromFile(filename);
	this->tilesize = tilesize;
}

TileSet::TileSet(std::string id, JsonBox::Value& v, EntityManager* mgr) : Entity(id)
{
	this->load(v, mgr);
}

void TileSet::load(JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	this->tilesize = o["tilesize"].getInteger();
	this->tex.loadFromFile(o["filename"].getString());
	if(o.find("animations") != o.end())
	{
		for(auto anim : o["animations"].getArray())
		{
			JsonBox::Object animO = anim.getObject();
			this->animations[animO["name"].getString()] =
				Animation(animO["x"].getInteger(), animO["y"].getInteger(), animO["len"].getInteger());
		}
	}
}
