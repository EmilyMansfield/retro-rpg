#ifndef GUI_TEXT_HPP
#define GUI_TEXT_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "gui.hpp"
#include "font.hpp"

// gui::Text is a visual representation of an std::string drawn using
// a gui::Font. The text may have a colour, like sf::Text, but each ascii
// character may also have a filled background determined by a universal
// background colour. To facillitate easy text colour changing the text
// in the font is white (we can then multiply by the intended colour),
// but then we can't include the background colour in the font. Instead
// we separately draw a background under all ascii glyphs. Non-ascii
// glyphs are geometrical characters, and cannot have their colour changed.
namespace gui
{
class Text : public sf::Drawable, public sf::Transformable
{
	private:

	// String of text to display. May contain non-ascii values
	std::string text;
	// Font to draw text in
	const gui::Font* font;
	// Colour of the text
	sf::Color textCol;
	// Background colour of all ascii text elements. Non-ascii elements
	// (i.e. those above the 0x80 range) are used to print geometry pieces,
	// which do not fill the entire glyph bounds and so handle their own
	// backgrounds
	sf::Color backgroundCol;
	// 1x1 texture used to print the background
	sf::Texture backgroundTex;

	// Array of vertices corresponding to the bounds of the glyphs to draw
	sf::VertexArray vertices;
	// Array of bounds of ascii glyphs
	sf::VertexArray backgroundVertices;

	// Width and height of the produced text
	sf::FloatRect bounds;

	// Regenerate the geometry of the text, populating the vertex arrays
	// and calculating the bounds
	void generateGeometry();

	// Construct a textured quad with position pos  and colour col from a glyph
	void glyphToQuad(const sf::Glyph& g, sf::Vertex* quad,
		const sf::Vector2f& pos, const sf::Color& col = sf::Color::Black);

	public:

	Text() : font(nullptr) {}
	Text(const std::string& text, const gui::Font& font,
		const sf::Color& backgroundCol = gui::Style::bg,
		const sf::Color& textCol = gui::Style::fg);

	// Override sf::Drawable::draw so the text can be printed using window.draw()
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	// Get a global position vector (after sf::Transformable transforms)
	// to the index-th character in the string
	sf::Vector2f findCharacterPos(size_t index) const;

	// Why are we using getters and setters here?
	// Changing most of the variables requires a geometry regeneration
	// before the change is visible, which we can force if we make
	// the variables private and only allow changes through setters
	// The bounds should also not be modifiable, so we use a getter
	// for those too. (But no setters, of course!)
	const sf::Color&	getColor()			const;
	const sf::Color&	getBackgroundColor()const;
	const gui::Font*	getFont()			const;
	sf::FloatRect		getLocalBounds()	const;
	sf::FloatRect		getGlobalBounds()	const;
	const std::string&	getString()			const;

	void setColor(const sf::Color& color);
	void setBackgroundColor(const sf::Color& color);
	void setFont(const gui::Font& font);
	void setString(const std::string& string);
};
}

#endif /* GUI_TEXT_HPP */
