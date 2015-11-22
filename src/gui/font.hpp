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

	// We could implement the font loading via the TileSet class
	// since they share a similar structure, but we can make some
	// further assumptions about a font (namely ascii order) which
	// stop us from needing a JSON file, unlike TileSet. Instead,
	// we'll just use an sf::Texture and will load everything manually
	sf::Texture tex;

	// Size of each character, in pixels
	unsigned int characterSize;

	// Number of characters on each row of the sheet
	unsigned int width;

	public:

	// Load the font from an image file which has the characters from
	// 0x20 to 0x9f arranged in a grid
	bool loadFromFile(const std::string& filename, unsigned int characterSize = 8);

	Font() {}
	Font(const std::string& filename, unsigned int characterSize = 8);

	// The benefit of using a getter here instead of making tex public
	// is that we don't want the texture of a font to change, because
	// then the rest of the object would become invalid
	const sf::Texture& getTexture() const;

	// Return an sf::Glyph object corresponding to the extended ascii character
	// value codePoint
	sf::Glyph getGlyph(unsigned int codePoint) const;

	// Return the distance between consecutive lines of text
	float getLineSpacing() const;
};
}
#endif /* BITMAP_FONT_HPP */
