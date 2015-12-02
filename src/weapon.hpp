#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include "JsonBox.h"

#include "item.hpp"

class EntityManager;

class Weapon : public Item
{
	public:

	int damage;

	// Constructors
	Weapon(const std::string& id,
		   const std::string& name,
		   const std::string& description,
		   int damage);
	Weapon(const std::string& id,
		   const JsonBox::Value& v,
		   EntityManager* mgr);

	void load(const JsonBox::Value& v, EntityManager* mgr);
};

#endif /* WEAPON_HPP */
