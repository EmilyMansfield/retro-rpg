#ifndef GUI_MESSAGE_BOX_HPP
#define GUI_MESSAGE_BOX_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "text.hpp"

namespace gui
{
// The gui::MessageBox class takes a string of arbitrary length to display,
// and splits it up into pages of text that fit into a given box size. It also
// adds a border around each page of text.
class MessageBox
{
	private:

	sf::IntRect dimensions;
	std::vector<gui::Text> pages;

	// Take a string and split it into a number of line each
	// fitting within the dimensions of the IntRect
	std::vector<std::string> alignString(const std::string& str);

	public:

	MessageBox() {}
	MessageBox(const sf::IntRect& dimensions, const std::string& text);
};
}
#endif /* GUI_MESSAGE_BOX_HPP */
