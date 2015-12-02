#include <string>
#include <utility>

#include "door.hpp"
#include "item.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"

Door::Door(const std::string& id,
		   const std::string& description,
		   const std::pair<std::string, std::string>& areas,
		   int locked,
		   Item* key) : 
	Entity(id),
	description(description),
	locked(locked),
	key(key),
	areas(areas) {}

Door::Door(const std::string& id,
		   const JsonBox::Value& v,
		   EntityManager* mgr) :
	Entity(id)
{
	this->load(v, mgr);
}

void Door::load(const JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	this->description = o["description"].getString();
	this->locked = o["locked"].getInteger();
	if(o.find("key") != o.end())
	{
		this->key = mgr->getEntity<Item>(o["key"].getString());
	}
	JsonBox::Array a = o["areas"].getArray();
	if(a.size() == 2)
	{
		this->areas.first = a[0].getString();
		this->areas.second = a[1].getString();
	}

	return;
}
