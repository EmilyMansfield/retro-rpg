#include <memory>
#include <utility>
#include <SFML/Graphics.hpp>
#include <JsonBox.h>

#include "renderer.hpp"
#include "mover.hpp"
#include "overworldable.hpp"

virtual void Overworldable::setPosition(const sf::Vector2f& pos)
{
	if(renderer) renderer->setPosition(pos);
	if(mover) mover->setPosition(pos);
}
virtual sf::Vector2f Overworldable::getPosition() const
{
	if(mover) return mover->getPosition();
	else return sf::Vector2f(0,0);
}

virtual void Overworldable::loadMover(const JsonBox::Value& v)
{
	JsonBox::Object o = v.getObject();
	if(mover != nullptr) mover->load(o["mover"].getObject());
}

template<typename T, typename... U>
void Overworldable::attachRenderer(U&&... u)
{
	renderer.reset(new T(std::forward<U>(u)...));
}
template<typename T, typename... U>
void Overworldable::attachMover(U&&... u)
{
	mover.reset(new T(std::forward<U>(u)...));
}
