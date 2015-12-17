#include <string>

#include "entity.hpp"
#include "entity_manager.hpp"

class Item;
class Weapon;
class Armor;
class Creature;
class Area;
class Door;
class TileSet;

Entity::Entity(const std::string& id) :
	id(id)
{

}

Entity::~Entity()
{

}

template <class T>
bool Entity::isA() const
{
	return id.substr(0, entityToString<T>().size()) == entityToString<T>();
}

template bool Entity::isA<Item>() const;
template bool Entity::isA<Weapon>() const;
template bool Entity::isA<Armor>() const;
template bool Entity::isA<Creature>() const;
template bool Entity::isA<Area>() const;
template bool Entity::isA<Door>() const;
template bool Entity::isA<TileSet>() const;
