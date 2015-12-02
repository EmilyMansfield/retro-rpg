#include <SFML/Graphics.hpp>

#include "font.hpp"

gui::Font::Font(const std::string& filename, unsigned int characterSize) :
	Font()
{
	loadFromFile(filename, characterSize);
}

bool gui::Font::loadFromFile(const std::string& filename,
	unsigned int characterSize)
{
	bool status = mTex.loadFromFile(filename);
	mCharacterSize = characterSize;
	mWidth = mTex.getSize().x / characterSize;
	return status;
}

const sf::Texture& gui::Font::getTexture() const
{
	return mTex;
}

sf::Glyph gui::Font::getGlyph(unsigned int codePoint) const
{
	sf::Glyph g;
	// Allow for ASCII from space up to DEL (last standard character) and then
	// allow for another custom 64 characters after that
	if(codePoint < 0x20 || codePoint > 0xC0) return g;
	g.textureRect = sf::IntRect(
		mCharacterSize * (codePoint % mWidth),
		mCharacterSize * ((codePoint-0x20) / mWidth),
		mCharacterSize,
		mCharacterSize);
	g.bounds = sf::FloatRect(0.0f, 0.0f, mCharacterSize, mCharacterSize);
	g.advance = mCharacterSize;

	return g;
}

float gui::Font::getLineSpacing() const
{
	// Fixed width fonts are separated by their character size
	// both horizontally and vertically
	return mCharacterSize;
}
