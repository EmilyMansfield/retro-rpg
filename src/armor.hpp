#ifndef ARMOR_HPP
#define ARMOR_HPP

#include <string>
#include "JsonBox.h"

#include "item.hpp"

class EntityManager;

class Armor : public Item
{
	public:

	int defense;

	// Constructors
	Armor(const std::string& id, const std::string& name,
		  const std::string& description, int defense);
	Armor(const std::string& id, const JsonBox::Value& v, EntityManager* mgr);

	// Load the armor from the Json value
	void load(const JsonBox::Value& v, EntityManager* mgr);
};

#endif /* ARMOR_HPP */
