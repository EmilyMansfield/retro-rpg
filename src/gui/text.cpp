#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

#include "font.hpp"
#include "text.hpp"

void gui::Text::glyphToQuad(const sf::Glyph& g, sf::Vertex* quad,
	const sf::Vector2f& pos, const sf::Color& col)
{
	// By treating the pointer as an array we may easily access
	// the vertices of the quad
	// 3---2
	// |   |
	// 0---1
	quad[0].position = sf::Vector2f(pos.x, pos.y + g.bounds.height);
	quad[1].position = sf::Vector2f(pos.x + g.bounds.width, pos.y + g.bounds.height);
	quad[2].position = sf::Vector2f(pos.x + g.bounds.width, pos.y);
	quad[3].position = sf::Vector2f(pos.x, pos.y);

	quad[0].texCoords = sf::Vector2f(g.textureRect.left, g.textureRect.top + g.textureRect.height);
	quad[1].texCoords = sf::Vector2f(g.textureRect.left + g.textureRect.width, g.textureRect.top + g.textureRect.height);
	quad[2].texCoords = sf::Vector2f(g.textureRect.left + g.textureRect.width, g.textureRect.top);
	quad[3].texCoords = sf::Vector2f(g.textureRect.left, g.textureRect.top);

	quad[0].color = col;
	quad[1].color = col;
	quad[2].color = col;
	quad[3].color = col;
}

void gui::Text::generateGeometry()
{
	// Each tile is a quad
	vertices.setPrimitiveType(sf::Quads);
	backgroundVertices.setPrimitiveType(sf::Quads);

	// Calculate number of non-newline glyphs
	// (newlines do not require a quad)
	// Double-count all border glyphs because they'll need both
	// background and foreground pieces
	size_t count = 0;
	for(size_t i = 0; i < text.size(); ++i)
	{
		if(text[i] != '\n') ++count;
		if(0x80 <= text[i] <= 0x88) ++count;
	}
	// Allocate space in the vertex array for them
	vertices.resize(4 * count);

	// Calculate number of non-newline ascii glyphs
	// (non-ascii characters do not require a background quad)
	count = 0;
	for(size_t i = 0; i < text.size(); ++i)
	{
		if(text[i] != '\n' && text[i] < 0x80) ++count;
	}
	// Allocate space for them
	backgroundVertices.resize(4 * count);

	// Construct the vertex arrays and calculate the bounds
	bounds = sf::FloatRect(0,0,0,0);
	float x = 0; // Current 'cursor' position, if you were typing the text
	float y = 0;
	// i is the index of the character in the text
	// j is the index of the background quad
	// k is the index of the glyph quad
	for(size_t i = 0, j = 0, k = 0; i < text.size(); ++i)
	{
		// Newlines reset the cursor to the start of the next line
		// On Windows, '\n' moves the the next line but does not
		// move to the start, and an additional '\r' character is
		// required. We ignore '\r' characters and adopt the POSIX
		// convention (Linux, OSX, etc.) where '\n' does both.
		if(text[i] == '\n')
		{
			x = 0;
			y += font->getLineSpacing();
		}
		else
		{
			// Add the glyph to the vertex array
			sf::Glyph g = font->getGlyph(text[i] & 0xff);
			glyphToQuad(g, &vertices[4*k], sf::Vector2f(x, y), textCol);
			++k; // Just added a glyph to the array

			// If a border character then add the background glyph too
			// This is necessary because border glyphs only have partially
			// filled backgrounds
			if(0x80 <= (text[i] & 0xff) && (text[i] & 0xff) <= 0x88)
			{
				// Background glyphs for borders are 0x20 later
				sf::Glyph bg = font->getGlyph((text[i] & 0xff) + 0x20);
				glyphToQuad(bg, &vertices[4*k], sf::Vector2f(x, y), backgroundCol);
				++k;
			}
			// Add to the background glyphs if an ascii character
			else if((text[i] & 0xff) < 0x80)
			{
				sf::Vertex* quad = &backgroundVertices[4*j];

				quad[0].position = sf::Vector2f(x, y + g.bounds.height);
				quad[1].position = sf::Vector2f(x + g.bounds.width, y + g.bounds.height);
				quad[2].position = sf::Vector2f(x + g.bounds.width, y);
				quad[3].position = sf::Vector2f(x, y);

				quad[0].texCoords = sf::Vector2f(0, 0);
				quad[1].texCoords = sf::Vector2f(1, 0);
				quad[2].texCoords = sf::Vector2f(1, 1);
				quad[3].texCoords = sf::Vector2f(0, 1);

				++j; // Just added a background to the array
			}

			// Increase the current position by the glyph bounds
			x += g.bounds.width;
			// Adjust the bounds if necessary (i.e. keep track of the greatest x)
			if(x > bounds.width) bounds.width = x;
		}
	}
	// No need to keep track of greatest y for vertical bounds,
	// since y only ever increases
	bounds.height = y;
}

gui::Text::Text(const std::string& text, const gui::Font& font,
	const sf::Color& backgroundCol, const sf::Color& textCol)
{
	this->text = text;
	this->font = &font;
	this->backgroundCol = backgroundCol;
	this->textCol = textCol;
	this->backgroundTex.create(1, 1);
	unsigned char pixels[4] = {
		backgroundCol.r,
		backgroundCol.g,
		backgroundCol.b,
		backgroundCol.a
	};
	this->backgroundTex.update(pixels);

	generateGeometry();
}

void gui::Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
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

	// Set the glyph texture
	states.texture = &font->getTexture();

	// Draw the glyph vertex array
	target.draw(vertices, states);
}

sf::Vector2f gui::Text::findCharacterPos(size_t index) const
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
			pos.y += font->getLineSpacing();
		}
		// This is equivalent to increasing by the font's
		// characterSize in this case, but this is more natural
		// (you'll get a speedup doing that though)
		pos.x += font->getGlyph(text[i]).advance;
	}

	// Apply sf::Transformable transform to local position
	return getTransform().transformPoint(pos);
}

const sf::Color& gui::Text::getColor()			const { return textCol; }
const sf::Color& gui::Text::getBackgroundColor()const { return backgroundCol; }
const gui::Font* gui::Text::getFont()			const { return font; }
const std::string& gui::Text::getString()		const { return text; }
sf::FloatRect gui::Text::getLocalBounds()		const { return bounds; }
sf::FloatRect gui::Text::getGlobalBounds()		const
{
	return getTransform().transformRect(bounds);
}

void gui::Text::setColor(const sf::Color& color)
{
	textCol = color;
	generateGeometry();
}

void gui::Text::setBackgroundColor(const sf::Color& color)
{
	backgroundCol = color;
	unsigned char pixels[4] = {color.r, color.g, color.b, color.a};
	backgroundTex.update(pixels);
	generateGeometry();
}

void gui::Text::setFont(const gui::Font& font)
{
	this->font = &font;
	generateGeometry();
}

void gui::Text::setString(const std::string& string)
{
	text = string;
	generateGeometry();
}
