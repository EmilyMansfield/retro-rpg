#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "JsonBox.h"
#include <string>

class EntityManager;

class Entity
{
	public:

	std::string id;

	Entity(const std::string& id) : id(id) {}

	// Destructor must be made virtual as all derived classes are
	// treated as Entity in the EntityManager
	virtual ~Entity() {}

	// Pure virtual function stops Entity from being instantiated and forces it
	// to be implemented in all derived types
	virtual void load(const JsonBox::Value& v, EntityManager* mgr) = 0;

	template <class T>
	bool isA() const;
};

#endif /* ENTITY_HPP */
