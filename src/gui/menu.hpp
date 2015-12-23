#ifndef MENU_HPP
#define MENU_HPP

#include <utility>
#include <SFML/Graphics.hpp>

#include "gui.hpp"
#include "text.hpp"

class Font;

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

enum class Direction { UP, DOWN, LEFT, RIGHT };
enum class NavigationMode { STOP, LOOP, ADVANCE, PAGE };

class Menu : public sf::Drawable, public sf::Transformable
{
	private:

	// Contains all entries in string format, ordered from left
	// to right and top to bottom as in the example, along with
	// their callbacks. We maintain this vector to allow for
	// dynamic insertion and deletion of elements.
	std::vector<std::pair<std::string, void (*)(void*, size_t)>> mEntries;
	// Preprocessed entries which have been aligned and split into
	// lines. This only needs to be regenerated when the entries
	// themselves are changed
	std::vector<std::string> mAlignedLines;
	// Pages of the menu formed from stitching the aligned lines together
	std::vector<gui::Text> mPages;
	size_t mCurrentPage;
	// Number of entries in width x height format,
	// *not* rows x columns 
	sf::Vector2u mAlignment;
	// Bounds of each individual entry, in width x height
	sf::Vector2u mEntrySize;
	// Font to use
	const gui::Font* mFont;
	// The currently selected element and its selector character
	size_t mSelectedEntry;
	unsigned char mSelectorCharacter;

	// We have to store these ourselves because generateGeometry
	// will reset the text properties
	sf::Color mBackgroundCol;
	sf::Color mTextCol;

	// If true, no attempt will be made to fit the entries into a box,
	// they will simply be truncated to fit
	bool mTrim;

	void formatEntries();
	void generatePage(size_t start, size_t end);
	void generateGeometry();

	public:

	Menu() {}
	Menu(const sf::Vector2u& alignment, const sf::Vector2u& entrySize,
		 const gui::Font& font, const sf::Color& backgroundCol = gui::Style::bg,
		 const sf::Color& textCol = gui::Style::fg);

	// Mark an entry as selected by adding a selector before its entry,
	// by default `*`. Can be a non-ascii character.
	void select(size_t index, unsigned char selector = '*');
	// Move the selector in the specified direction using the specified
	// navigation mode, which determines the behaviour when the selector
	// reaches the end of a row or column
	// STOP - Do not move any more
	// LOOP - Reset to the start of the row or column
	// ADVANCE - Reset to the start of the next row or column
	void navigate(gui::Direction dir, gui::NavigationMode xMode,
		gui::NavigationMode yMode);
	size_t getSelected() const;

	// Call the callback associated with the currently selected entry
	void activate(void* ptr);

	void addEntry(const std::string& entry, void (*callback)(void*, size_t));

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void setBackgroundColor(const sf::Color& textCol);
	void setColor(const sf::Color& textCol);
	const sf::Color& getBackgroundColor() const;
	const sf::Color& getColor() const;

	// Return the total size of the menu, including borders,
	// measured in characters
	sf::Vector2u getSize() const;

	size_t getPage() const;
	size_t numPages() const;
	void setPage(size_t page);

	void setTrim(bool trim);
};
}

#endif /* MENU_HPP */
