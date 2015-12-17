#ifndef OVERWORLDABLE_HPP
#define OVERWORLDABLE_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include <JsonBox.h>

#include "renderer.hpp"
#include "mover.hpp"

class Overworldable
{
	public:

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<Mover> mover;

	virtual void setPosition(const sf::Vector2f& pos);
	virtual sf::Vector2f getPosition() const;

	virtual void loadMover(const JsonBox::Value& v);

	template<typename T, typename... U>
	void attachRenderer(U&&... u);

	template<typename T, typename... U>
	void attachMover(U&&... u);
};

#endif /* OVERWORLDABLE_HPP */
