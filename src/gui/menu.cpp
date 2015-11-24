#include <utility>
#include <sfml/Graphics.hpp>

#include "menu.hpp"
#include "message_box.hpp"

gui::Menu::Menu(const sf::Vector2u alignment, const sf::Vector2u& entrySize,
		const gui::Font& font)
{
	this->alignment = alignment;
	this->entrySize = entrySize;
	this->font = &font;
	generateGeometry();
}

void gui::Menu::generateGeometry()
{
	// First split each entry into lines which fit within the desired bounds
	std::vector<std::vector<std::string>> lines;
	for(auto entry : entries)
		lines.push_back(gui::alignString(entry.first, entrySize.x));

	// If laid out from left to right and top to bottom, then the
	// quotient of (number of entries) / (entries per row) will give
	// the number of completed rows, and the remainder will give the
	// number of entries on the partially completed row.
	unsigned int numRows = alignment.x / entrySize.x;
	unsigned int partialRow = alignment.x % entrySize.x;
	// We now concatenate the first entrySize.y lines of each
	// group of alignment.x entries. As an example if alignment is (3,1)
	// and entrySize is (5, 2) we will transform
	// {
	// 		{"opt1", "line2"},
	// 		{"opti2","line2"},
	// 		{"op3", "line2", "line3"}
	// }
	// to
	// {
	// 		{" opt1   opti2  op3   "},
	// 		{" line2  line2  line2 "}
	// }
	// Note that we will also add the correct padding, and will discard any
	// text outside of the bounds (if each entry were a messagebox, they would
	// be on another page).
}

void gui::Menu::select(unsigned int index, unsigned char selector = '*')
{
	generateGeometry();
}

void gui::Menu::activate(unsigned int index)
{
	entries.at(index).second(index);
}

void gui::Menu::addEntry(const std::string& entry, void (*callback)(int))
{
	entries.push_back(std::make_pair<entry, callback>);
	generateGeometry();
}
