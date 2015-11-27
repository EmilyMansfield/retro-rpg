#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_title.hpp"

void GameStateTitle::handleEvent(sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		// Handle menu navigation
		if(event.key.code == sf::Keyboard::Up)
			titleMenu.navigate(gui::Direction::UP, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
		else if(event.key.code == sf::Keyboard::Down)
			titleMenu.navigate(gui::Direction::DOWN, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
		// Open a menu option
		else if(event.key.code == sf::Keyboard::Return)
			titleMenu.activate(this);
	}
}

void GameStateTitle::handleInput(float dt)
{
}

void GameStateTitle::update(float dt)
{
}

void GameStateTitle::draw(sf::RenderWindow& window, float dt) const
{
	window.setView(this->view);
	window.draw(titleMenu);
}

void GameStateTitle::callbackContinue(int index)
{

}
void GameStateTitle::callbackQuit(int index)
{
	state.reset();
}
