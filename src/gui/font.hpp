#ifndef BITMAP_FONT_HPP
#define BITMAP_FONT_HPP

#include <SFML/Graphics.hpp>

// We'll be dealing entirely with fixed-width bitmap fonts as they'll
// fit more with the aesthetic of the game, and we can use them to
// create boxes and dialogues using non-ascii characters. Our Font
// class will resemble the sf::Font class, but it will lack many of
// the irrelevant functions. (Unfortunately sf::Font has no virtual
// methods, so we can't just redefine it and get a text class for free)
namespace gui
{
class Font
{
	private:

	// We could implement the font loading via the TileSet class,
	// since they share a similar structure, but we can make some
	// further assumptions about a font (namely ASCII order) which
	// stop us from needing a JSON file, unlike TileSet.
	sf::Texture tex;
	unsigned int characterSize;

	public:

	Font() {}

	bool loadFromFile(const std::string& filename,
		unsigned int characterSize = 8)
	{
		tex.loadFromFile(filename);
		this->characterSize = characterSize;
	}

	Font(const std::string& filename,
		unsigned int characterSize = 8) : Font()
	{
		loadFromFile(filename, characterSize);
	}

	const sf::Texture& getTexture(unsigned int characterSize = 8) const
	{
		return tex;
	}

	sf::Glyph getGlyph(unsigned int codePoint, unsigned int characterSize, bool bold) const
	{
		sf::Glyph g;
		// Allow for ASCII from space up to DEL (last standard character) and then allow for
		// another custom 32 characters up to that
		if(codePoint < 0x20 || codePoint > 0xA0) return g;
		// Font sheet is in 4 rows of 32=0x20, and each character is a characterSize^2 square
		g.textureRect = sf::IntRect(
			characterSize * (codePoint % 0x20),
			characterSize * ((codePoint-0x20) / 0x20),
			characterSize,
			characterSize);
		g.bounds = sf::FloatRect(0.0f, 0.0f, characterSize, characterSize);
		g.advance = characterSize;

		return g;
	}

	float getKerning(unsigned int first, unsigned int second, unsigned int characterSize) const
	{
		// Fixed width fonts don't have any fancy kerning
		return 0.0f;
	}

	float getLineSpacing(unsigned int characterSize) const
	{
		// Fixed width fonts are separated by their character size
		// both horizontally and vertically
		return characterSize;
	}

	float getUnderlinePosition(unsigned int characterSize) const
	{
		return 0.0f;
	}

	float getUnderlineThickness(unsigned int characterSize) const
	{
		return 0.0f;
	}
};
}
#endif /* BITMAP_FONT_HPP */
