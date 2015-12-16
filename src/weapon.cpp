#include <string>
#include <JsonBox.h>

#include "weapon.hpp"
#include "item.hpp"
#include "entity_manager.hpp"

Weapon::Weapon(const std::string& id,
			   const std::string& name,
			   const std::string& description,
			   int damage) :
	Item(id, name, description),
	damage(damage) {}

Weapon::Weapon(const std::string& id,
			   const JsonBox::Value& v,
			   EntityManager* mgr) :
	Item(id, v, mgr)
{
	this->load(v, mgr);
}

void Weapon::load(const JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	this->damage = o["damage"].getInteger();

	return;
}
