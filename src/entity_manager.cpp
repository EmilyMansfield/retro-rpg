#include <string>
#include <map>

#include "entity_manager.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "armor.hpp"
#include "creature.hpp"
#include "area.hpp"
#include "door.hpp"
#include "tile_set.hpp"

EntityManager::EntityManager() {}

EntityManager::~EntityManager()
{
	for(auto& entity : this->data)
	{
		delete entity.second;
	}
}

template <class T>
void EntityManager::loadJson(const std::string& filename)
{
	JsonBox::Value v;
	v.loadFromFile(filename);

	JsonBox::Object o = v.getObject();
	for(auto entity : o)
	{
		std::string key = entity.first;
		this->data[key] = dynamic_cast<Entity*>(new T(key, entity.second, this));
	}
}

template <class T>
T* EntityManager::getEntity(const std::string& id) const
{
	// The id prefix should match to the type T, so take the
	// first characters of the id up to the length of the
	// prefix and compare the two
	if(id.substr(0, entityToString<T>().size()) == entityToString<T>())
		return dynamic_cast<T*>(this->data.at(id));
	else
		return nullptr;
}

// Template specialisations
template <> std::string entityToString<Item>() { return "item"; }
template <> std::string entityToString<Weapon>() { return "weapon"; }
template <> std::string entityToString<Armor>() { return "armor"; }
template <> std::string entityToString<Creature>() { return "creature"; }
template <> std::string entityToString<Area>() { return "area"; }
template <> std::string entityToString<Door>() { return "door"; }
template <> std::string entityToString<TileSet>() { return "tileset"; }

// Template instantiations
template void EntityManager::loadJson<Item>(const std::string&);
template void EntityManager::loadJson<Weapon>(const std::string&);
template void EntityManager::loadJson<Armor>(const std::string&);
template void EntityManager::loadJson<Creature>(const std::string&);
template void EntityManager::loadJson<Area>(const std::string&);
template void EntityManager::loadJson<Door>(const std::string&);
template void EntityManager::loadJson<TileSet>(const std::string&);

template Item* EntityManager::getEntity<Item>(const std::string&) const;
template Weapon* EntityManager::getEntity<Weapon>(const std::string&) const;
template Armor* EntityManager::getEntity<Armor>(const std::string&) const;
template Creature* EntityManager::getEntity<Creature>(const std::string&) const;
template Area* EntityManager::getEntity<Area>(const std::string&) const;
template Door* EntityManager::getEntity<Door>(const std::string&) const;
template TileSet* EntityManager::getEntity<TileSet>(const std::string&) const;
