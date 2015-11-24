#ifndef MENU_HPP
#define MENU_HPP

#include <utility>
#include <SFML/Graphics.hpp>

#include "message_box.hpp"

namespace gui
{
// A Menu consists of an ordered set of gui::Text entries, each
// of which can be selected via the activate function.
// Each entry has an associated callback function which is
// called if that entry is activated, and which accepts
// a single argument equal to the index of the entry. The callback
// should return void.

// Each entry is created from an std::string and a callback, and
// the string is converted into a gui::Text object automatically
// according to the chosen alignment of the Menu. The alignment
// describes the number of items in each horizontal row of the
// Menu, and the number of items in each vertical column. The
// sizes of each individual gui::Text is also determined by the
// Menu's bounds, which describes the height and width of each
// gui::Text entry.
// e.g., with bounds of 8x2, alignment of 2x3 we would have
// the following 22x8 structure
// /--------------------\
// | Option 1  Option 2 |
// | Option 3 *Opt 4    |
// | Opt 5     Option 6 |
// \--------------------/
// and with bounds of 10x2, alignment of 1x3, the following
// 14x10 structure
// /------------\
// | This is a  |
// | long one.  |
// | One line   |
// |            |
// | Padding is |
// | not shrunk |
// \----------- /
//
// Note that horizontal padding is added on both sides of each option,
// to make room for the selector * which can be added to an entry via
// the select function and which marks the currently selected entry.

// All dimensions are given in characters, not in pixels. Character
// size is determined by the font (since our fonts are fixed size,
// if you're using sf::Font you'll have to specify characterSize
// in the constructor).
class Menu : public sf::Drawable, public sf::Transformable
{
	private:

	// Contains all entries in string format, ordered from left
	// to right and top to bottom as in the example, along with
	// their callbacks. We maintain this vector to allow for
	// dynamic insertion and deletion of elements.
	std::vector<std::pair<std::string, void (*)(int)>> entries;
	// The overall menu is a single gui::MessageBox object
	// created by stitching each individual entry together
	// in the correct pattern
	gui::MessageBox msgBox;
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
