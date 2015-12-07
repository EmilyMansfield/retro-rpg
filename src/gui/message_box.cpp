#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "gui.hpp"
#include "message_box.hpp"
#include "text.hpp"

gui::MessageBox::MessageBox(const sf::Vector2u& dimensions,
							const std::string& text,
							const gui::Font& font,
							const sf::Color& backgroundCol,
							const sf::Color& textCol) :
	mDimensions(dimensions),
	mFont(&font),
	mCurrentPage(0),
	mBackgroundCol(backgroundCol),
	mTextCol(textCol)
{
	createPages(gui::alignString(text, dimensions.x));
}

void gui::MessageBox::createPages(const std::vector<std::string>& lines)
{
	unsigned int borderWidth = mDimensions.x + 2;
	unsigned int& maxWidth = mDimensions.x;
	unsigned int& maxHeight = mDimensions.y;

	std::string page = gui::Border::genTop(borderWidth);

	// If the lines vector is empty, we'll just make a blank box
	if(lines.size() == 0)
	{
		for(size_t i = 0; i < maxHeight; ++i)
		{
			page += gui::Border::genRow(borderWidth);
		}
		page += gui::Border::genBottom(borderWidth);
		mPages.push_back(gui::Text(page, *mFont, mBackgroundCol, mTextCol));
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
			page += gui::Border::genBottom(borderWidth);
			// Add the page to the list of pages and reset
			mPages.push_back(gui::Text(page, *mFont, mBackgroundCol, mTextCol));
			page = gui::Border::genTop(borderWidth);
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
		for(size_t i = lines.size() % maxHeight; i < maxHeight; ++i)
		{
			page += gui::Border::genRow(borderWidth);
		}
		page += gui::Border::genBottom(borderWidth);
		mPages.push_back(gui::Text(page, *mFont, mBackgroundCol, mTextCol));
	}
}

void gui::MessageBox::setPage(unsigned int page)
{
	if(page >= mPages.size())
		page = mPages.size() % page;
	mCurrentPage = page;
}

unsigned int gui::MessageBox::getPage() const
{
	return mCurrentPage;
}

unsigned int gui::MessageBox::numPages() const 
{
	return mPages.size();
}

void gui::MessageBox::draw(sf::RenderTarget& target,
						   sf::RenderStates states) const
{
	states.transform *= getTransform();
	mPages.at(mCurrentPage).draw(target, states);
}

sf::Vector2u gui::MessageBox::getSize() const
{
	return mDimensions + sf::Vector2u(2, 2);
}

void gui::MessageBox::setText(const std::string& text)
{
	mPages.clear();
	createPages(gui::alignString(text, mDimensions.x));
	mCurrentPage = 0;
}
