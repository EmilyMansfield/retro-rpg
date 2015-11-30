#ifndef OVERWORLDABLE_HPP
#define OVERWORLDABLE_HPP

#include <memory>
#include <utility>
#include <SFML/Graphics.hpp>

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
		if(mover) renderer->setPosition(pos);
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
