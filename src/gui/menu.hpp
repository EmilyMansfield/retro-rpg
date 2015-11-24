#ifndef MENU_HPP
#define MENU_HPP

#include <utility>
#include <SFML/Graphics.hpp>

#include "text.hpp"

namespace gui
{
// A Menu consists of an ordered set of entries, each of which can be
// selected via the activate function. Each entry has an associated
// callback function which is called if that entry is activated, and
// which accepts a single argument equal to the index of the entry.
// The callback should return void.

// Each entry is created from an std::string and a callback, and the
// strings are concatenated and converted into a gui::Text object
// automatically according to the chosen alignment of the Menu. The
// alignment describes the width and height of the menu, in entries.
// The size of each entry is determined by the entrySize argument,
// which is measured in characters. e.g., with entrySize of 8x2,
// alignment of 2x3 we would have the following 22x8 structure

// /--------------------\
// | Option 1  Option 2 |
// | Option 3 *Opt 4    |
// | Opt 5     Option 6 |
// \--------------------/

// and with entrySize of 10x2, alignment of 1x3, the following 14x10
// structure

// /------------\
// | This is a  |
// | long one.  |
// | One line   |
// |            |
// | Padding is |
// | not shrunk |
// \----------- /

// Note that horizontal padding is added on both sides of each option,
// to make room for the selector * which can be added to an entry via
// the select function and which marks the currently selected entry.

// Character size is determined by the font (since our fonts are fixed
// size, if you're using sf::Font you'll have to specify characterSize
// in the constructor).

class Menu : public sf::Drawable, public sf::Transformable
{
	private:

	// Contains all entries in string format, ordered from left
	// to right and top to bottom as in the example, along with
	// their callbacks. We maintain this vector to allow for
	// dynamic insertion and deletion of elements.
	std::vector<std::pair<std::string, void (*)(int)>> entries;
	// The overall menu is a single gui::Text object
	// created by stitching each individual entry together
	// in the correct pattern
	gui::Text text;
	// Number of entries in width x height format,
	// *not* rows x columns 
	sf::Vector2u alignment;
	// Bounds of each individual entry, in width x height
	sf::Vector2u entrySize;
	// Font to use
	const gui::Font* font;

	void generateGeometry();

	public:

	Menu() {}
	Menu(const sf::Vector2u alignment, const sf::Vector2u& entrySize,
		const gui::Font& font);

	// Mark an entry as selected by adding a selector before its entry,
	// by default `*`. Can be a non-ascii character.
	void select(unsigned int index, unsigned char selector = '*');
	// Call the callback associated with the index, and pass through the
	// index of the option as an argument
	void activate(unsigned int index);

	void addEntry(const std::string& entry, void (*callback)(int));

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
}

#endif /* MENU_HPP */
