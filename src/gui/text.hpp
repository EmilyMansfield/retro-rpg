#ifndef BITMAP_TEXT_HPP
#define BITMAP_TEXT_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#include "font.hpp"

namespace gui
{
class Text : public sf::Drawable, public sf::Transformable
{
	private:

	std::string text;
	unsigned int characterSize;
	gui::Font* font;
	sf::Color col;
	sf::Color backgroundCol;
	sf::Texture backgroundTex;

	sf::VertexArray vertices;
	sf::VertexArray backgroundVertices;
	sf::FloatRect bounds;

	void generateGeometry()
	{
		// Each tile is a quad
		vertices.setPrimitiveType(sf::Quads);
		backgroundVertices.setPrimitiveType(sf::Quads);

		// Calculate number of non-newline glyphs
		size_t count = 0;
		for(size_t i = 0; i < text.size(); ++i)
		{
			if(text[i] != '\n') ++count;
		}
		// Allocate space in the vertex array for them
		vertices.resize(4 * count);

		// Calculate number of non-newline ascii glyphs
		count = 0;
		for(size_t i = 0; i < text.size(); ++i)
		{
			if(text[i] != '\n' && text[i] < 0x80) ++count;
		}
		// Allocate space for them
		backgroundVertices.resize(4 * count);

		// Construct the vertex arrays and calculate the bounds
		bounds = sf::FloatRect(0,0,0,0);
		float x = 0;
		float y = 0;
		for(size_t i = 0, j = 0, k = 0; i < text.size(); ++i)
		{
			if(text[i] == '\n')
			{
				x = 0;
				y += font->getLineSpacing(characterSize);
			}
			else
			{
				// Add the glyph to the vertex array
				sf::Glyph g = font->getGlyph(text[i] & 0xff, characterSize, false);
				// Pointer to the current quad. By treating this as an
				// array we may easily access the vertices of the quad
				// 3---2
				// |   |
				// 0---1
				sf::Vertex* quad = &vertices[4*k];

				quad[0].position = sf::Vector2f(x, y + g.bounds.height);
				quad[1].position = sf::Vector2f(x + g.bounds.width, y + g.bounds.height);
				quad[2].position = sf::Vector2f(x + g.bounds.width, y);
				quad[3].position = sf::Vector2f(x, y);

				quad[0].texCoords = sf::Vector2f(g.textureRect.left, g.textureRect.top + g.textureRect.height);
				quad[1].texCoords = sf::Vector2f(g.textureRect.left + g.textureRect.width, g.textureRect.top + g.textureRect.height);
				quad[2].texCoords = sf::Vector2f(g.textureRect.left + g.textureRect.width, g.textureRect.top);
				quad[3].texCoords = sf::Vector2f(g.textureRect.left, g.textureRect.top);

				++k;
				// Add to the background glyphs if an ascii character
				if((text[i] & 0xff) < 0x80)
				{
					quad = &backgroundVertices[4*j];

					quad[0].position = sf::Vector2f(x, y + g.bounds.height);
					quad[1].position = sf::Vector2f(x + g.bounds.width, y + g.bounds.height);
					quad[2].position = sf::Vector2f(x + g.bounds.width, y);
					quad[3].position = sf::Vector2f(x, y);

					quad[0].texCoords = sf::Vector2f(0, 0);
					quad[1].texCoords = sf::Vector2f(1, 0);
					quad[2].texCoords = sf::Vector2f(1, 1);
					quad[3].texCoords = sf::Vector2f(0, 1);

					++j;
				}

				// Increase the current position by the glyph bounds
				x += g.bounds.width;
				// Adjust the bounds if necessary
				if(x > bounds.width) bounds.width = x;
			}
		}
		bounds.height = y;
	}

	public:

	Text() : font(nullptr) {}
	Text(const std::string& text, Font& font,
		unsigned int characterSize = 8)
	{
		this->text = text;
		this->font = &font;
		this->characterSize = characterSize;
		this->backgroundCol = sf::Color(0,0,0,0);
		this->col = sf::Color(0xff,0xff,0xff,0xff);
		this->backgroundTex.create(1, 1);
		unsigned char pixels[4] = {0, 0, 0, 0};
		this->backgroundTex.update(pixels);

		generateGeometry();
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// Apply the transformation from sf::Transformable
		states.transform *= getTransform();

		// Set the background texture
		if(backgroundCol.a > 0)
		{
			states.texture = &backgroundTex;

			// Draw the background array
			target.draw(backgroundVertices, states);
		}

		// Set the texture
		states.texture = &font->getTexture(characterSize);

		// Draw the vertex array
		target.draw(vertices, states);
	}

	sf::Vector2f findCharacterPos(size_t index) const
	{
		// Don't exceed text bounds
		if(index > text.size()) index = text.size();

		// Local position of the character in the text
		sf::Vector2f pos;

		// Even though we're using fixed-width fonts, we can't simply
		// multiply the index by the character size because there
		// might be newlines (tabs aren't supported though)
		for(size_t i = 0; i < index; ++i)
		{
			if(text[i] == '\n')
			{
				pos.x = 0;
				pos.y += font->getLineSpacing(characterSize);
			}
			// Might as well be general and support non-fixed-width too
			pos.x += font->getGlyph(text[i], characterSize, false).advance;
		}

		// Apply sf::Transformable transform to local position
		return getTransform().transformPoint(pos);
	}

	unsigned int getCharacterSize() const
	{
		return characterSize;
	}

	const sf::Color& getColor() const
	{
		return col;
	}

	const sf::Color& getBackgroundColor() const
	{
		return backgroundCol;
	}

	const gui::Font* getFont() const
	{
		return font;
	}

	sf::FloatRect getLocalBounds() const
	{
		return bounds;
	}

	sf::FloatRect getGlobalBounds() const
	{
		return getTransform().transformRect(bounds);
	}

	const std::string& getString() const
	{
		return text;
	}

	unsigned int getStyle() const
	{
		return 0;
	}

	void setCharacterSize(unsigned int size)
	{
		characterSize = size;

		generateGeometry();
	}

	void setColor(sf::Color& color)
	{
		col = color;
	}

	void setBackgroundColor(const sf::Color& color)
	{
		backgroundCol = color;
		unsigned char pixels[4] = {color.r, color.g, color.b, color.a};
		backgroundTex.update(pixels);
	}

	void setFont(gui::Font& font)
	{
		this->font = &font;

		generateGeometry();
	}

	void setString(const std::string& string)
	{
		text = string;

		generateGeometry();
	}
};
}

#endif /* BITMAP_TEXT_HPP */
