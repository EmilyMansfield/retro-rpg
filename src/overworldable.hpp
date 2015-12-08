#ifndef OVERWORLDABLE_HPP
#define OVERWORLDABLE_HPP

#include <memory>
#include <utility>
#include <SFML/Graphics.hpp>
#include "JsonBox.h"

#include "renderer.hpp"
#include "mover.hpp"

class Overworldable
{
	public:

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Mover> mover;

	virtual void setPosition(const sf::Vector2f& pos)
	{
		if(renderer) renderer->setPosition(pos);
		if(mover) mover->setPosition(pos);
	}
	virtual sf::Vector2f getPosition() const
	{
		if(mover) return mover->getPosition();
		else return sf::Vector2f(0,0);
	}

	virtual void loadMover(const JsonBox::Value& v)
	{
		JsonBox::Object o = v.getObject();
		if(mover != nullptr) mover->load(o["mover"].getObject());
	}

	template<typename T, typename... U>
	void attachRenderer(U&&... u)
	{
		renderer.reset(new T(std::forward<U>(u)...));
	}
	template<typename T, typename... U>
	void attachMover(U&&... u)
	{
		mover.reset(new T(std::forward<U>(u)...));
	}
};

#endif /* OVERWORLDABLE_HPP */
