#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <map>
#include <utility>
#include <JsonBox.h>

#include "entity_manager.hpp"

class Item;
class Weapon;
class Armor;

class ItemEntry
{
	private:

	Item* ptr;

	public:

	size_t count;

	ItemEntry(Item* ptr,
			  size_t count) :
		ptr(ptr),
		count(count) {}

	ItemEntry(const std::string& id,
			  size_t count,
			  EntityManager* mgr) :
		ptr(mgr->getEntity<Item>(id)),
		count(count) {}

	// Allows implicit casting to any derived type of Item*
	template <class T>
	operator T*() const { return dynamic_cast<T*>(ptr); }
};

class Inventory
{
	private:

	std::vector<ItemEntry> items;

	// Load an array of [id, count] pairs
	void load(const JsonBox::Value& v, EntityManager* mgr);

	// Return a JSON representation of all the items of the type T
	template <typename T>
	JsonBox::Array jsonArray() const;

	public:

	// Load the inventory from a JSON value
	Inventory(const JsonBox::Value& v,
			  EntityManager* mgr);
	Inventory() {}

	// Add an item to the inventory
	void add(Item* item, size_t count);

	// Remove the specified number of items from the inventory
	void remove(Item* item, size_t count);

	// Returns the count of the specified item
	size_t count(Item* item) const;
	template <typename T>
	size_t count(size_t n) const;

	// Return the nth item in the storage list
	Item* get(size_t n) const;
	template <typename T>
	T* get(size_t n) const;

	// Remove all items from the inventory
	void clear();

	// Return total number if items
	size_t size() const;

	// Get a Json object representation of the inventory
	JsonBox::Object getJson() const;

	Inventory& operator+=(const Inventory& rhs);
	Inventory& operator-=(const Inventory& rhs);
};

inline Inventory operator+(Inventory lhs, const Inventory& rhs)
{
	lhs += rhs;
	return lhs;
}
inline Inventory operator-(Inventory lhs, const Inventory& rhs)
{
	lhs -= rhs;
	return lhs;
}

#endif /* INVENTORY_HPP */
