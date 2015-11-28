#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace gui
{
	// No enum here because most uses require
	// a string-compatible type, and + doesn't
	// convert chars to std::strings
	namespace Border {
		static const char* topLeft = "\x80";
		static const char* top = "\x87";
		static const char* topRight = "\x81";
		static const char* right = "\x84";
		static const char* bottomRight = "\x82";
		static const char* bottom = "\x85";
		static const char* bottomLeft = "\x83";
		static const char* left = "\x86";

		static inline std::string genTop(size_t length)
		{
			return topLeft + std::string(length-2, top[0]) + topRight + "\n";
		}
		static inline std::string genBottom(size_t length)
		{
			return bottomLeft + std::string(length-2, bottom[0]) + bottomRight;
		}
		static inline std::string genRow(size_t length)
		{
			return left + std::string(length-2, ' ') + right + "\n";
		}
		static inline std::string surround(const std::string& str)
		{
			return left + str + right;
		}
	}

	namespace Style {
		static const sf::Color fg(0xff, 0xff, 0xff);
		static const sf::Color bg(0x00, 0x40, 0x58);
	}

	// Take a string and split it into a number of lines each
	// fitting within the given width
	std::vector<std::string> alignString(const std::string& str,
		unsigned int maxWidth);
}

#endif /* GUI_HPP */
