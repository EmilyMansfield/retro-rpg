#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>

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
			return gui::Border::topLeft +
				std::string(length-2, gui::Border::top[0]) + 
				gui::Border::topRight + "\n";
		}
		static inline std::string genBottom(size_t length)
		{
			return gui::Border::bottomLeft +
				std::string(length-2, gui::Border::bottom[0]) + 
				gui::Border::bottomRight;
		}
		static inline std::string genRow(size_t length)
		{
			return gui::Border::left +
				std::string(length-2, ' ') + 
				gui::Border::right + "\n";
		}
		static inline std::string surround(const std::string& str)
		{
			return gui::Border::left + str + gui::Border::right;
		}
	}

	namespace Style {
		static const sf::Color fg(0xff, 0xff, 0xff);
		static const sf::Color bg(0x00, 0x40, 0x58);
	}
}

#endif /* GUI_HPP */
