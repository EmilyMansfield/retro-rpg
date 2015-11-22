#include <SFML/Graphics.hpp>

#include "font.hpp"

bool gui::Font::loadFromFile(const std::string& filename, unsigned int characterSize)
{
	tex.loadFromFile(filename);
	this->characterSize = characterSize;
	this->width = tex.getSize().x / characterSize;
}

gui::Font::Font(const std::string& filename, unsigned int characterSize) : Font()
{
	loadFromFile(filename, characterSize);
}

const sf::Texture& gui::Font::getTexture() const
{
	return tex;
}

sf::Glyph gui::Font::getGlyph(unsigned int codePoint) const
{
	sf::Glyph g;
	// Allow for ASCII from space up to DEL (last standard character) and then allow for
	// another custom 32 characters up to that
	if(codePoint < 0x20 || codePoint > 0xA0) return g;
	g.textureRect = sf::IntRect(
		characterSize * (codePoint % width),
		characterSize * ((codePoint-0x20) / width),
		characterSize,
		characterSize);
	g.bounds = sf::FloatRect(0.0f, 0.0f, characterSize, characterSize);
	g.advance = characterSize;

	return g;
}

float gui::Font::getLineSpacing() const
{
	// Fixed width fonts are separated by their character size
	// both horizontally and vertically
	return characterSize;
}
