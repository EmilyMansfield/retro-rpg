#include <SFML/Graphics.hpp>
#include <vector>
#include <iterator>
#include <string>
#include <sstream>

#include "message_box.hpp"
#include "text.hpp"

std::vector<std::string> gui::MessageBox::alignString(const std::string& str) const
{
	std::vector<std::string> alignedLines;

	// We'll add a border, so the dimensions the string should fit
	// in are smaller than the dimensions of the message box
	size_t maxWidth = dimensions.width - 2;

	// First we split up str into lines, determined by manually placed
	// newlines in the string. We do this before splitting into words
	// because otherwise they'd be discarded and text such as
	// Wait...
	// What is that?!
	// would be impossible to create.
	std::istringstream lineSS(str);
	std::vector<std::string> lines;
	for(std::string line; std::getline(lineSS, line); )
		lines.push_back(line);

	// Now we process each line at a time and split the line into words
	for(auto line : lines)
	{
		// Here the first entry is an iterator which extracts elements
		// of type std::string from the stream wordSS using the << operator,
		// and the second entry is an iterator pointing to the end of
		// the stream (this is always the empty constructor)
		std::istringstream wordSS(line);
		std::vector<std::string> words {
			std::istream_iterator<std::string>(wordSS),
			std::istream_iterator<std::string>()
		};

		std::string alignedLine;

		// Now we can iterate over the words and insert newlines when
		// they would be too long to fit in the box
		size_t width = 0;
		for(auto i = words.begin(); i != words.end(); ++i)
		{
			std::string w = *i;
			// If we're at the start of the line and the word doesn't
			// fit, it will never fit so give up and just hyphenate it
			if(width == 0 && w.size() > maxWidth)
			{
				// Only take maxWidth-1 character because we want to put
				// a hypen on the end, e.g. if maxWidth = 8
				// thisisa-
				// longword
				std::string partA = w.substr(0, maxWidth-1);
				std::string partB = w.substr(maxWidth-1);
				alignedLine = partA + "-";
				alignedLines.push_back(alignedLine);
				// Now empty the line, replace the word by the second part
				// and try again. This way we can hyphenate the same word
				// multiple times if it's really long
				alignedLine.clear();
				*i = partB;
				width = 0;
				--i; continue;
			}
			// Otherwise if the word doesn't fit then add a newline,
			// reset to the start of the next line, and try again
			// +1 takes into account the space that would be added
			// if this could fit onto the line
			else if(width != 0 && width + w.size() + 1 > maxWidth)
			{
				alignedLines.push_back(alignedLine);
				alignedLine.clear();
				width = 0;
				// No 'redo' or 'retry' in C++, but we can fake one
				// by decrementing the iterator then continuing
				--i; continue;
			}
			// The word fits, so preface with some whitespace if needed
			// then add the word. Whitespace is only required if we aren't
			// at the start of a line
			else
			{
				std::string toAdd = (width == 0 ? "" : " ") + w;
				alignedLine += toAdd;
				width += toAdd.size();
			}
		}

		// Now add the alignedLine to the vector of aligned lines
		alignedLines.push_back(alignedLine);
	}

	return alignedLines;
}

void gui::MessageBox::createPages(const std::vector<std::string>& lines)
{
	size_t maxHeight = dimensions.height - 2;
	size_t maxWidth = dimensions.width - 2;

	std::string page = topBorder(dimensions.width);

	// If the lines vector is empty, we'll just make a blank box
	if(lines.size() == 0)
	{
		for(size_t i = 0; i < maxHeight; ++i)
			page += rowBorder(dimensions.width);
		page += bottomBorder(dimensions.width);
		pages.push_back(gui::Text(page, *font));
		return;
	}

	// Otherwise there are lines of text so we can add them
	for(size_t i = 0; i < lines.size(); ++i)
	{
		// There are maxHeight lines, so this is true on the last line
		if(i % maxHeight == maxHeight-1)
		{
			// Expand the line to the width of the box
			std::string padding;
			if(lines[i].size() < maxWidth)
			{
				for(size_t j = 0; j < maxWidth-lines[i].size(); ++j)
					padding += " ";
			}
			// Surround the line with border to pieces and add it to the page
			page += "\x86" + lines[i] + padding + "\x84\n";
			// Add the bottom border of the page
			page += bottomBorder(dimensions.width);
			// Add the page to the list of pages and reset
			pages.push_back(gui::Text(page, *font));
			page = topBorder(dimensions.width);
		}
		else
		{
			std::string padding;
			if(lines[i].size() < maxWidth)
			{
				for(size_t j = 0; j < maxWidth-lines[i].size(); ++j)
					padding += " ";
			}
			page += "\x86" + lines[i] + padding + "\x84\n";
		}
	}
	// We'll still have a partial page unless the number of
	// lines is divisible by the maximum height
	if(lines.size() % maxHeight != 0)
	{
		// Add padding lines to make the page the desired height
		for(size_t i = lines.size() % maxHeight; i <= maxHeight; ++i)
		{
			page += rowBorder(dimensions.width);
		}
		page += bottomBorder(dimensions.width);
		pages.push_back(gui::Text(page, *font));
	}
}

std::string gui::MessageBox::topBorder(size_t length)
{
	std::string border = "\x80";
	for(size_t i = 1; i < length-1; ++i)
	{
		border += "\x87";
	}
	border += "\x81\n";
	return border;
}

std::string gui::MessageBox::bottomBorder(size_t length)
{
	std::string border = "\x83";
	for(size_t i = 1; i < length-1; ++i)
	{
		border += "\x85";
	}
	border += "\x82";
	return border;
}

std::string gui::MessageBox::rowBorder(size_t length)
{
	std::string border = "\x86";
	for(size_t i = 1; i < length-1; ++i)
	{
		border += " ";
	}
	border += "\x84\n";
	return border;
}

gui::MessageBox::MessageBox(const sf::IntRect& dimensions,
	const std::string& text, gui::Font& font)
{
	this->dimensions = dimensions;
	this->font = &font;
	createPages(alignString(text));
	this->currentPage = 0;
}
