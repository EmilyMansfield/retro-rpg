#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "message_box.hpp"
#include "text.hpp"

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
	const std::string& text, const gui::Font& font)
{
	this->dimensions = dimensions;
	this->font = &font;
	this->currentPage = 0;
	createPages(gui::alignString(text, dimensions.width-2));
	// Set the background colours
	for(auto& page : pages)
	{
		page.setBackgroundColor(sf::Color(0x00, 0x40, 0x58));
	}
}

void gui::MessageBox::setPage(unsigned int page)
{
	if(page >= pages.size())
		page = pages.size() % page;
	currentPage = page;
}
unsigned int gui::MessageBox::getPage() const { return currentPage; }

void gui::MessageBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	pages.at(currentPage).draw(target, states);
}
