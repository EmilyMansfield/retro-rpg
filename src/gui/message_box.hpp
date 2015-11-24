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
class MessageBox : public sf::Drawable, public sf::Transformable
{
	private:

	sf::IntRect dimensions;
	const gui::Font* font;
	std::vector<gui::Text> pages;
	unsigned int currentPage;

	// Given a list of lines, form those line into pages according to the
	// dimensions of the IntRect and then generate the geometry for them
	// via gui::Text
	void createPages(const std::vector<std::string>& lines);

	// Return border characters of given length, e.g.
	// topBorder(8) = /------\
	// bottomBorder(5) = \---/
	std::string topBorder(size_t length);
	std::string bottomBorder(size_t length);
	std::string rowBorder(size_t length);

	public:

	MessageBox() {}
	MessageBox(const sf::IntRect& dimensions, const std::string& text,
		const gui::Font& font);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Sets the current page and checks bounds too, looping if too high
	void setPage(unsigned int page);
	// Have to use a getter since we used a setter
	unsigned int getPage() const;
};
}
#endif /* GUI_MESSAGE_BOX_HPP */
