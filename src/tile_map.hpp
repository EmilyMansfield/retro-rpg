#ifndef TILE_MAP_HPP
#define TILE_MAP_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "JsonBox.h"

#include "tile_set.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
	private:

	TileSet* tileset;
	sf::VertexArray verts;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Apply the transformation from sf::Transformable
		states.transform *= this->getTransform();

		// Set the texture to the tileset
		states.texture = &tileset->tex;

		// Draw the vertex array
		target.draw(this->verts, states);
	}

	public:

	unsigned int w; // Number of columns (x axis)
	unsigned int h; // Number of rows (y axis)

	std::vector<unsigned int> map;

	TileMap() {}
	TileMap(JsonBox::Array& a, TileSet* tileset)
	{
		// Height is the length of the first array
		this->h = a.size();
		if(this->h > 0)
		{
			// Assume the width is the length of the first subarray
			this->w = a[0].getArray().size();
			for(auto row : a)
			{
				for(auto tile : row.getArray())
				{
					// Add the tiles to the map
					this->map.push_back(tile.getInteger());
				}
			}
		}

		// Set the tileset
		this->tileset = tileset;
		unsigned int ts = this->tileset->tilesize;

		// Construct a vertex array from the map

		// Each tile is a quad
		this->verts.setPrimitiveType(sf::Quads);
		// Hence there are 4 vertices for each tile
		this->verts.resize(4 * this->w * this->h);

		// Populate the vertex array
		for(unsigned int y = 0; y < this->h; ++y)
		{
			for(unsigned int x = 0; x < this->w; ++x)
			{
				// Convert the tile value to a (u,v) position
				// in the tileset
				int u = this->map[y * this->w + x] % (this->tileset->tex.getSize().x / ts);
				int v = this->map[y * this->w + x] / (this->tileset->tex.getSize().x / ts);

				// Pointer to first vertex of the quad corresponding to
				// the tile. By treating it as an array we can easily
				// access the next vertices and instead can treat this
				// vertex pointer as the entire quad.
				sf::Vertex* quad = &this->verts[4 * (y * this->w + x)];

				// Set the quad's vertex positions
				// 3---2
				// |   |
				// 0---1
				quad[0].position = sf::Vector2f( x    * ts,  y    * ts);
				quad[1].position = sf::Vector2f((x+1) * ts,  y    * ts);
				quad[2].position = sf::Vector2f((x+1) * ts, (y+1) * ts);
				quad[3].position = sf::Vector2f( x    * ts, (y+1) * ts);

				// Set the quad's texture coordinates
				quad[0].texCoords = sf::Vector2f( u    * ts,  v    * ts);
				quad[1].texCoords = sf::Vector2f((u+1) * ts,  v    * ts);
				quad[2].texCoords = sf::Vector2f((u+1) * ts, (v+1) * ts);
				quad[3].texCoords = sf::Vector2f( u    * ts, (v+1) * ts);
			}
		}
	}
};
#endif /* TILE_MAP_HPP */
