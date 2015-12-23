#include <memory>
#include <SFML/Graphics.hpp>
#include <JsonBox.h>

#include "renderer.hpp"
#include "mover.hpp"
#include "overworldable.hpp"

void Overworldable::setPosition(const sf::Vector2f& pos)
{
	if(renderer) renderer->setPosition(pos);
	if(mover) mover->setPosition(pos);
}
sf::Vector2f Overworldable::getPosition() const
{
	if(mover) return mover->getPosition();
	else return sf::Vector2f(0,0);
}

void Overworldable::loadMover(const JsonBox::Value& v)
{
	JsonBox::Object o = v.getObject();
	if(mover != nullptr) mover->load(o["mover"].getObject());
}

Overworldable::~Overworldable()
{

}
