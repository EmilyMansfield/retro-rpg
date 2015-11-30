#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>

#include "tile_set.hpp"

class Renderer : public sf::Drawable
{
	public:

	virtual void setPosition(const sf::Vector2f& pos) = 0;
	virtual void setFrame(const std::string& animStr, float interp) = 0;

	virtual ~Renderer() {}
};
#endif /* RENDERER_HPP */
