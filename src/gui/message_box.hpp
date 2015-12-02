#ifndef GUI_MESSAGE_BOX_HPP
#define GUI_MESSAGE_BOX_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "gui.hpp"
#include "text.hpp"

class Font;

namespace gui
{
// The gui::MessageBox class takes a string of arbitrary length to display,
// and splits it up into pages of text that fit into a given box size. It also
// adds a border around each page of text.
class MessageBox : public sf::Drawable, public sf::Transformable
{
	private:

	sf::Vector2u mDimensions;
	const gui::Font* mFont;
	std::vector<gui::Text> mPages;
	size_t mCurrentPage;

	sf::Color mBackgroundCol;
	sf::Color mTextCol;

	// Given a list of lines, form those line into pages according to the
	// dimensions of the Vector2u and then generate the geometry for them
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
	MessageBox(const sf::Vector2u& dimensions,
			   const std::string& text,
			   const gui::Font& font,
			   const sf::Color& backgroundCol = gui::Style::bg,
			   const sf::Color& textCol = gui::Style::fg);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Sets the current page and checks bounds too, looping if too high
	void setPage(unsigned int page);
	// Have to use a getter since we used a setter
	unsigned int getPage() const;
	unsigned int numPages() const;
	sf::Vector2u getSize() const;

	// Change the text without affecting any other properties
	void setText(const std::string& text);
};
}
#endif /* GUI_MESSAGE_BOX_HPP */
