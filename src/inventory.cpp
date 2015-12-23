#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <JsonBox.h>

#include "inventory.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "armor.hpp"
#include "entity_manager.hpp"

Inventory::Inventory(const JsonBox::Value& v,
					 EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	load(o["items"], mgr);
	load(o["weapons"], mgr);
	load(o["armor"], mgr);
}

void Inventory::load(const JsonBox::Value& v, EntityManager* mgr)
{
	for(auto item : v.getArray())
	{
		std::string itemId = item.getArray()[0].getString();
		int quantity = item.getArray()[1].getInteger();
		items.push_back(ItemEntry(itemId, quantity, mgr));
	}
}

template <typename T>
JsonBox::Array Inventory::jsonArray() const
{
	JsonBox::Array a;
	for(auto item : this->items)
	{
		// Skip if the id does not match to the type T
		if(static_cast<Item*>(item)->id.substr(0, entityToString<T>().size()) != entityToString<T>()) continue;
		// Otherwise add the item to the array
		JsonBox::Array pair;
		pair.push_back(JsonBox::Value(static_cast<Item*>(item)->id));
		pair.push_back(JsonBox::Value((int)item.count));
		a.push_back(JsonBox::Value(pair));
	}

	return a;
}

void Inventory::add(Item* item, size_t count)
{
	for(auto& it : items)
	{
		Item* ptr = it;
		if(ptr->id == item->id)
		{
			it.count += count;
			return;
		}
	}
	items.push_back(ItemEntry(item, count));
}

void Inventory::remove(Item* item, size_t count)
{
	// Iterate through the items, and if they are found then decrease
	// the quantity by the quantity removed
	for(auto it = items.begin(); it != items.end(); ++it)
	{
		if(static_cast<Item*>(*it)->id == item->id)
		{
			(*it).count -= count;
			if((*it).count < 1) items.erase(it);
			return;
		}
	}
}

Item* Inventory::get(size_t n) const
{
	if(n < items.size())
		return items[n];
	else
		return nullptr;
}

template <typename T>
T* Inventory::get(size_t n) const
{
	// Unless T=Item, there's no guarantee that there are
	// n of an item type
	for(size_t i = 0, j = 0; i < items.size(); ++i)
	{
		if(!static_cast<Item*>(items[i])->isA<T>()) continue;
		if(j == n)
		{
			return items[j];
		}
	}
	return nullptr;
}

size_t Inventory::count(Item* item) const
{
	for(auto it : this->items)
	{
		if(static_cast<Item*>(it)->id == item->id)
			return it.count;
	}
	return 0;
}

template <typename T>
size_t Inventory::count(size_t n) const
{
	return count(get<T>(n));
}

void Inventory::clear()
{
	items.clear();
}

size_t Inventory::size() const
{
	return items.size();
}

JsonBox::Object Inventory::getJson() const
{
	JsonBox::Object o;

	o["items"] = JsonBox::Value(jsonArray<Item>());
	o["weapons"] = JsonBox::Value(jsonArray<Weapon>());
	o["armor"] = JsonBox::Value(jsonArray<Armor>());

	return o;
}

Inventory& Inventory::operator+=(const Inventory& rhs)
{
	for(auto it : rhs.items) add(it, it.count);
	return *this;
}
Inventory& Inventory::operator-=(const Inventory& rhs)
{
	for(auto it : rhs.items) remove(it, it.count);
	return *this;
}

// Template instantiations
template ItemEntry::operator Item*() const;
template ItemEntry::operator Weapon*() const;
template ItemEntry::operator Armor*() const;

template JsonBox::Array Inventory::jsonArray<Item>() const;
template JsonBox::Array Inventory::jsonArray<Weapon>() const;
template JsonBox::Array Inventory::jsonArray<Armor>() const;

template size_t Inventory::count<Item>(size_t) const;
template size_t Inventory::count<Weapon>(size_t) const;
template size_t Inventory::count<Armor>(size_t) const;

template Item* Inventory::get<Item>(size_t) const;
template Weapon* Inventory::get<Weapon>(size_t) const;
template Armor* Inventory::get<Armor>(size_t) const;
