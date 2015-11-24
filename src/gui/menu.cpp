#include <utility>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "menu.hpp"
#include "message_box.hpp"

void gui::Menu::generateGeometry()
{
	// First split each entry into lines which fit within the desired bounds
	std::vector<std::vector<std::string>> alignedEntries;
	for(auto entry : entries)
		alignedEntries.push_back(gui::alignString(entry.first, entrySize.x));

	// If laid out from left to right and top to bottom, then the
	// quotient of (number of entries) / (entries per row) will give
	// the number of completed rows, and the remainder will give the
	// number of entries on the partially completed row.
	unsigned int numRows = entries.size() / alignment.x;
	unsigned int partialRow = entries.size() % alignment.x;
	// We now concatenate the first entrySize.y lines of each
	// group of alignment.x entries. As an example if alignment is (3,1)
	// and entrySize is (5, 2) we will transform
	// {
	// 		{"opt1"},
	// 		{"opti2","line2"},
	// 		{"op3", "line2", "line3"}
	// }
	// to
	// {
	// 		{" opt1  ", "       "},
	// 		{" opti2 ", " line2 "},
	// 		{" op3   ", " line2 ", " line3 "}
	// }
	// and then to
	// {
	// 		{" opt1   opti2  op3   "},
	// 		{"        line2  line2 "}
	// }
	// Note that we will also add the correct padding, and will discard any
	// text outside of the bounds (if each entry were a messagebox, they would
	// be on another page).
	for(auto& entry : alignedEntries)
	{
		// First pad the existing rows
		for(size_t row = 0; row < entry.size(); ++row)
		{
			// Far left and right padding is 1, internal padding is 2,
			// which is equivalent to each entry being surrounded by 1
			// space. Each entry should also have additional right padding
			// so that it's the same length as entrySize.x
			for(size_t i = 0; i < entrySize.x - entry[row].size(); ++i)
				entry[row] += " ";
			entry[row] = " " + entry[row] + " ";
		}
		// Now add additional rows made of whitespace, if necessary
		for(size_t row = entry.size(); row < entrySize.y; ++row)
		{
			std::string line;
			for(size_t i = 0; i < entrySize.x + 2; ++i)
				line += " ";
			entry.push_back(line);
		}
	}
	// Now each entry is padded, they can be combined
	std::vector<std::string> alignedLines;
	// Note row is not a line, but rather a row of entries
	// which may span multiple lines
	for(unsigned int row = 0; row < numRows; ++row)
	{
		for(unsigned int line = 0; line < entrySize.y; ++line)
		{
			std::string alignedLine;
			for(unsigned int entry = 0; entry < alignment.x; ++entry)
			{
				alignedLine += alignedEntries[row * alignment.x + entry][line];
			}
			alignedLines.push_back(alignedLine);
		}
	}
	// Now do the last partial row, if there is one
	// if(partialRow > 0)
	// {
	// 	for(unsigned int line = 0; line < entrySize.y; ++line)
	// 	{
	// 		std::string alignedLine;
	// 		for(unsigned int entry = 0; entry < partialRow; ++entry)
	// 		{
	// 			alignedLine += alignedEntries[(numRows-1) * alignment.x + entry][line];
	// 		}
	// 	}
	// }

	// Now every line has been padded we can form a single string from them and
	// pass them to gui::MessageBox to get formatted output. gui::MessageBox
	// won't need to do any alignment (if everything worked properly) because
	// we've already ensured that everything is the right length, but it will
	// put a border around everything and generate the required gui::Text element.
	// 
	// The total number of lines we want gui::MessageBox to work with is
	// entrySize.y * [numRows + (1 if partial row)] + 2
	// Since each row has entrySize.y lines, there's no vertical padding,
	// there's numRows (+1) rows, and there's two extra lines for the border.
	//
	// The total width of the gui::MessageBox is
	// alignment.x * (entrySize.x + 2) + 2
	// since each entry is entrySize.x wide, they have two extra characters for
	// padding, there's alignment.x of them, and there's two more for the border.
	sf::IntRect msgBoxBounds(0, 0,
		alignment.x * (entrySize.x + 2) + 2,
		alignment.y * entrySize.y + 2);

	std::string msgBoxStr;
	// The number of aligned lines will not be equal to msgBoxBounds.y in general,
	// so we will iterate up to the minimum of the two and then fill the rest with
	// blank lines
	unsigned int bound = alignedLines.size();
	if(bound > msgBoxBounds.height) bound = msgBoxBounds.height;
	for(unsigned int i = 0; i < bound; ++i)
	{
		msgBoxStr += alignedLines[i];
		if(i != msgBoxBounds.height-1) msgBoxStr += "\n"; // Don't add newline to last line
	}
	std::string padding;
	for(unsigned int i = 0; i < msgBoxBounds.width; ++i)
		padding += " ";
	for(unsigned int i = bound; i < msgBoxBounds.height; ++i)
	{
		msgBoxStr += padding;
		if(i != msgBoxBounds.height-1) msgBoxStr += "\n";
	}

	// Finally we can create the gui::MessageBox
	msgBox = gui::MessageBox(msgBoxBounds, msgBoxStr, *font);
}

void gui::Menu::select(unsigned int index, unsigned char selector)
{
	generateGeometry();
}

void gui::Menu::activate(unsigned int index)
{
	entries.at(index).second(index);
}

void gui::Menu::addEntry(const std::string& entry, void (*callback)(int))
{
	entries.push_back(std::make_pair(entry, callback));
	generateGeometry();
}

void gui::Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	msgBox.draw(target, states);
}

gui::Menu::Menu(const sf::Vector2u alignment, const sf::Vector2u& entrySize,
		const gui::Font& font)
{
	this->alignment = alignment;
	this->entrySize = entrySize;
	this->font = &font;
	generateGeometry();
}
