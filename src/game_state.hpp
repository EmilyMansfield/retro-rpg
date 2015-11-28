#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "gui/font.hpp"

// Alternative is to manage fonts using the entity manager, but fonts
// are not really entities and this way we can keep the gui definitions
// separate from the rest of the game
static const gui::Font mainFont("assets/font.png");

class GameState
{
	public:

	virtual void handleEvent(sf::Event& event) = 0;
	virtual void handleInput(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow& window, float dt) const = 0;

	virtual ~GameState() {}	
};

#endif /* GAME_STATE_HPP */
