#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "gui.hpp"
#include "message_box.hpp"
#include "text.hpp"

void gui::MessageBox::createPages(const std::vector<std::string>& lines)
{
	size_t maxHeight = dimensions.y - 2;
	size_t maxWidth = dimensions.x - 2;

	std::string page = gui::Border::genTop(dimensions.x);

	// If the lines vector is empty, we'll just make a blank box
	if(lines.size() == 0)
	{
		for(size_t i = 0; i < maxHeight; ++i)
			page += gui::Border::genRow(dimensions.x);
		page += gui::Border::genBottom(dimensions.x);
		pages.push_back(gui::Text(page, *font, backgroundCol, textCol));
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
				padding = std::string(maxWidth - lines[i].size(), ' ');
			}
			// Surround the line with border to pieces and add it to the page
			page += gui::Border::surround(lines[i]+padding) + "\n";
			// Add the bottom border of the page
			page += gui::Border::genBottom(dimensions.x);
			// Add the page to the list of pages and reset
			pages.push_back(gui::Text(page, *font, backgroundCol, textCol));
			page = gui::Border::genTop(dimensions.x);
		}
		else
		{
			std::string padding;
			if(lines[i].size() < maxWidth)
			{
				padding = std::string(maxWidth-lines[i].size(), ' ');
			}
			page += gui::Border::surround(lines[i] + padding) + "\n";
		}
	}
	// We'll still have a partial page unless the number of
	// lines is divisible by the maximum height
	if(lines.size() % maxHeight != 0)
	{
		// Add padding lines to make the page the desired height
		for(size_t i = lines.size() % maxHeight; i <= maxHeight; ++i)
		{
			page += gui::Border::genRow(dimensions.x);
		}
		page += gui::Border::genBottom(dimensions.x);
		pages.push_back(gui::Text(page, *font, backgroundCol, textCol));
	}
}

gui::MessageBox::MessageBox(const sf::Vector2u& dimensions,
	const std::string& text, const gui::Font& font,
	const sf::Color& backgroundCol, const sf::Color& textCol)
{
	this->dimensions = dimensions;
	this->font = &font;
	this->currentPage = 0;
	this->backgroundCol = backgroundCol;
	this->textCol = textCol;
	createPages(gui::alignString(text, dimensions.x-2));
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
