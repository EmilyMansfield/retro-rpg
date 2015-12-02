#ifndef ENTITY_RENDERER_HPP
#define ENTITY_RENDERER_HPP

#include <SFML/Graphics.hpp>

#include "renderer.hpp"
#include "tile_set.hpp"

class EntityRenderer : public Renderer
{
	private:

	// Tilesheet containing the entity's texture frames
	TileSet* tiles;
	unsigned int ts;

	// Sprite used to display the entity
	sf::Sprite sprite;

	public:

	EntityRenderer() {}
	EntityRenderer(TileSet* tiles) :
		tiles(tiles),
		ts(tiles->tilesize)
	{
		this->sprite.setTexture(this->tiles->tex);
	}

	EntityRenderer* clone() const { return new EntityRenderer(*this); }

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setPosition(const sf::Vector2f& pos);
	void setFrame(const std::string& animStr, float interp);
};

#endif /* ENTITY_RENDERER_HPP */
