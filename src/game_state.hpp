#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class GameState
{
	public:

	virtual void handleInput(sf::Event& event) = 0;
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderWindow& window, float dt) const = 0;

	virtual ~GameState() {}	
};

#endif /* GAME_STATE_HPP */
