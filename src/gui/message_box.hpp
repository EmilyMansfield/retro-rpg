#ifndef GUI_MESSAGE_BOX_HPP
#define GUI_MESSAGE_BOX_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "text.hpp"
#include "font.hpp"

namespace gui
{
// The gui::MessageBox class takes a string of arbitrary length to display,
// and splits it up into pages of text that fit into a given box size. It also
// adds a border around each page of text.
class MessageBox
{
	private:

	sf::IntRect dimensions;
	gui::Font* font;

	// Take a string and split it into a number of line each
	// fitting within the dimensions of the IntRect
	std::vector<std::string> alignString(const std::string& str) const;

	// Given a list of lines, form those line into pages according to the
	// dimensions of the IntRect and then generate the geometry for them
	// via gui::Text
	void createPages(const std::vector<std::string>& lines);

	// Return border characters of given length, e.g.
	// topBorder(8) = /------\
	// bottomBorder(5) = \---/
	std::string topBorder(size_t length);
	std::string bottomBorder(size_t length);

	public:
	std::vector<gui::Text> pages;
	MessageBox() {}
	MessageBox(const sf::IntRect& dimensions, const std::string& text,
		gui::Font& font);
};
}
#endif /* GUI_MESSAGE_BOX_HPP */
