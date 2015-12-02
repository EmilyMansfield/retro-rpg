#include <string>
#include "JsonBox.h"

#include "armor.hpp"
#include "item.hpp"
#include "entity_manager.hpp"

Armor::Armor(const std::string& id,
			 const std::string& name,
			 const std::string& description,
			 int defense) :
	Item(id, name, description),
	defense(defense) {}

Armor::Armor(const std::string& id,
			 const JsonBox::Value& v,
			 EntityManager* mgr) :
	Item(id, v, mgr)
{
	this->load(v, mgr);
}

void Armor::load(const JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	this->defense = o["defense"].getInteger();

	return;
}
