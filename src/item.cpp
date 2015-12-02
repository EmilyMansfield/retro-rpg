#include <string>
#include "JsonBox.h"

#include "item.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"

Item::Item(const std::string& id,
		   const std::string& name,
		   const std::string& description) :
	Entity(id),
	name(name),
	description(description) {}

Item::Item(const std::string& id,
		   const JsonBox::Value& v,
		   EntityManager* mgr) :
	Entity(id)
{
	this->load(v, mgr);
}

void Item::load(const JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	this->name = o["name"].getString();
	this->description = o["description"].getString();

	return;
}
