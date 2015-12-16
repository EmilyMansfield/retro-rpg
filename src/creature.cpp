#include <string>
#include <cstdlib>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <JsonBox.h>

#include "creature.hpp"
#include "entity.hpp"
#include "inventory.hpp"
#include "weapon.hpp"
#include "armor.hpp"
#include "door.hpp"
#include "area.hpp"
#include "entity_manager.hpp"
#include "creature_mover.hpp"
#include "entity_renderer.hpp"

Creature::Creature(const std::string& id,
				   const std::string& name,
				   int hp,
				   int strength,
				   int agility,
				   double evasion,
				   unsigned int xp) :
	Entity(id),
	name(name),
	hp(hp),
	maxHp(hp),
	strength(strength),
	agility(agility),
	evasion(evasion),
	xp(xp),
	equippedWeapon(nullptr),
	equippedArmor(nullptr) {}

Creature::Creature(const std::string& id,
				   const JsonBox::Value& v,
				   EntityManager* mgr) :
	Entity(id)
{
	this->load(v, mgr);
}

Creature::Creature(const Creature& c) :
	Entity(c.id),
	name(c.name),
	hp(c.hp),
	maxHp(c.maxHp),
	strength(c.strength),
	agility(c.agility),
	evasion(c.evasion),
	xp(c.xp), 
	inventory(c.inventory),
	equippedWeapon(c.equippedWeapon),
	equippedArmor(c.equippedArmor),
	currentArea(c.currentArea)
{
	if(c.mover) mover.reset(c.mover->clone());
	if(c.renderer) renderer.reset(c.renderer->clone());
}

void Creature::equipWeapon(Weapon* weapon)
{
	this->equippedWeapon = weapon;

	return;
}

void Creature::equipArmor(Armor* armor)
{
	this->equippedArmor = armor;

	return;
}

Area* Creature::getAreaPtr(EntityManager* mgr) const
{
	return mgr->getEntity<Area>(this->currentArea);
}

int Creature::attack(Creature* target)
{
	// Damage done
	int damage = 0;

	if(double(std::rand()) / RAND_MAX  > target->evasion)
	{
		// Calculate attack based on strength and weapon damage
		int attack = this->strength + (this->equippedWeapon == nullptr ? 0 : this->equippedWeapon->damage);
		// Calculate defense based on agility and armor defense
		int defense = target->agility + (target->equippedArmor == nullptr ? 0 : target->equippedArmor->defense);
		// 1/32 chance of a critical hit
		if(std::rand() % 32 == 0)
		{
			// Ignore defense and do damage in range [attack/2, attack]
			damage = attack / 2 + std::rand() % (attack / 2 + 1);
		}
		else
		{
			// Normal hit so factor in defense
			int baseDamage = attack - defense / 2;
			// Do damage in range [baseDamage/4, baseDamage/2]
			damage = baseDamage / 4 + std::rand() % (baseDamage / 4 + 1);
			// If the damage is zero then have a 50% chance to do 1 damage
			if(damage < 1)
			{
				damage = std::rand() % 2;
			}
		}
		// Damage the target
		target->hp -= damage;
	}

	return damage;
}


int Creature::traverse(Door* door)
{
	int flag = 2;
	// Open the door if it is shut
	if(door->locked == 0)
	{
		door->locked = -1;
		flag = 2;
	}
	else if(door->locked > 0)
	{
		// Unlock and open the door if the creature has the key
		if(this->inventory.count(door->key))
		{
			door->locked = -1;
			flag = 1;
		}
		// Creature does not have key so door remains locked
		else
		{
			return 0;
		}
	}
	if(door->areas.first == this->currentArea)
	{
		this->currentArea = door->areas.second;
	}
	else if(door->areas.second == this->currentArea)
	{
		this->currentArea = door->areas.first;
	}

	return flag;
}

JsonBox::Object Creature::toJson() const
{
	JsonBox::Object o;
	o["name"] = JsonBox::Value(this->name);
	o["hp"] = JsonBox::Value(this->hp);
	o["hp_max"] = JsonBox::Value(this->maxHp);
	o["strength"] = JsonBox::Value(this->strength);
	o["agility"] = JsonBox::Value(this->agility);
	o["evasion"] = JsonBox::Value(this->evasion);
	o["xp"] = JsonBox::Value(int(this->xp));
	o["inventory"] = JsonBox::Value(this->inventory.getJson());
	o["equipped_weapon"] = JsonBox::Value(this->equippedWeapon == nullptr ? "nullptr" : this->equippedWeapon->id);
	o["equipped_armor"] = JsonBox::Value(this->equippedArmor == nullptr ? "nullptr" : this->equippedArmor->id);

	return o;
}

void Creature::load(const JsonBox::Value& v, EntityManager* mgr)
{
	JsonBox::Object o = v.getObject();
	this->name = o["name"].getString();
	this->hp = o["hp"].getInteger();
	if(o.find("hp_max") != o.end())
	{
		this->maxHp = o["hp_max"].getInteger();
	}
	else
	{
		this->maxHp = hp;
	}
	this->strength = o["strength"].getInteger();
	this->agility = o["agility"].getInteger();
	this->evasion = o["evasion"].getDouble();
	this->xp = o["xp"].getInteger();

	if(o.find("inventory") != o.end())
	{
		this->inventory = Inventory(o["inventory"], mgr);
	}
	if(o.find("equipped_weapon") != o.end())
	{
		std::string equippedWeaponName = o["equipped_weapon"].getString();
		this->equippedWeapon = equippedWeaponName == "nullptr" ? nullptr : mgr->getEntity<Weapon>(equippedWeaponName);
	}
	if(o.find("equipped_armor") != o.end())
	{
		std::string equippedArmorName = o["equipped_armor"].getString();
		this->equippedArmor = equippedArmorName == "nullptr" ? nullptr : mgr->getEntity<Armor>(equippedArmorName);
	}

	return;
}

void Creature::update(float dt)
{
	if(renderer && mover)
	{
		// Move creature according to velocity
		mover->update(dt);
		// Calculate the animation interpolation value by calculating
		// how far through a single tile of movement the sprite is
		sf::Vector2f p1 = mover->getPosition();
		sf::Vector2f p2(floor(p1.x), floor(p1.y));
		// One of these terms is zero, so this is equivalent to taking
		// max(fabs(p1.x-p2.x), fabs(p1.y-p2.y))
		float interp = fabs(p1.x-p2.x) + fabs(p1.y-p2.y);

		// id of the playing animation
		std::string animString;
		if(mover->isMoving())
		{
			animString = id + std::string("_walk_") + static_cast<char>(mover->getFacing());
		}
		else
		{
			animString = id + std::string("_idle_") + static_cast<char>(mover->getFacing());
		}
		renderer->setPosition(mover->getPosition());
		renderer->setFrame(animString, interp);
	}
}

void Creature::setPosition(const sf::Vector2f& pos)
{
	if(mover) mover->setPosition(pos);
}

void Creature::step(float dt, Direction dir, const TileMap& tm)
{
	if(mover) mover->step(dt, dir, tm);
}
