#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "JsonBox.h"

#include "tile_set.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
	private:

	TileSet* tileset;
	sf::VertexArray verts;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:

	unsigned int w; // Number of columns (x axis)
	unsigned int h; // Number of rows (y axis)
	unsigned int ts; // Width and height of each tile

	std::vector<unsigned int> map;

	TileMap() {}
	TileMap(JsonBox::Array& a, TileSet* tileset);

	unsigned int at(unsigned int x, unsigned int y);
	unsigned int at(sf::Vector2u pos);
};
#endif /* TILE_MAP_HPP */
