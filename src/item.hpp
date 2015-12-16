#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <JsonBox.h>

#include "entity.hpp"

class EntityManager;

class Item : public Entity
{
	public:

	// Name and description of the item
	std::string name;
	std::string description;

	// Constructors
	Item(const std::string& id,
		 const std::string& name,
		 const std::string& description);
	Item(const std::string& id, 
		 const JsonBox::Value& v,
		 EntityManager* mgr);

	// Load the item information from the JSON value
	virtual void load(const JsonBox::Value& v,
					  EntityManager* mgr);
};

#endif /* ITEM_HPP */
