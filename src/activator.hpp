#ifndef ACTIVATOR_HPP
#define ACTIVATOR_HPP

#include "overworldable.hpp"

class Creature;

class Activator : public Overworldable
{
	public:

	virtual void toggle(const Creature& user) = 0;
	virtual void set(const Creature& user, bool on) = 0;

	virtual ~Activator() {}
};

#endif /* ACTIVATOR_HPP */
