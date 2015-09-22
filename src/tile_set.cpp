#include <string>
#include "JsonBox.h"
#include <SFML/Graphics.hpp>

#include "tile_set.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"

TileSet::TileSet(std::string id, std::string filename) : Entity(id)
{
	this->tex.loadFromFile(filename);
}

TileSet::TileSet(std::string id, JsonBox::Value& v, EntityManager* mgr) : Entity(id)
{
	this->load(v, mgr);
}

void TileSet::load(JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	this->filename = o["filename"].getString();
	this->tex.loadFromFile(filename);
}
