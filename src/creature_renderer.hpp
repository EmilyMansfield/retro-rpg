#ifndef CREATURE_RENDERER_HPP
#define CREATURE_RENDERER_HPP

#include <SFML/Graphics.hpp>

#include "tile_set.hpp"

class CreatureRenderer : public sf::Drawable
{
	private:

	// Tilesheet containing the creature's texture frames
	TileSet* tiles;
	unsigned int ts;

	// Sprite used to display the creature
	sf::Sprite sprite;

	public:

	CreatureRenderer() {}
	CreatureRenderer(TileSet* tiles)
	{
		this->tiles = tiles;
		this->sprite.setTexture(this->tiles->tex);
		this->ts = this->tiles->tilesize;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setPosition(const sf::Vector2f& pos);
	void setFrame(const std::string& animStr, float interp);
};

#endif /* CREATURE_RENDERER_HPP */
