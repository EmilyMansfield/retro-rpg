#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_menu_status.hpp"
#include "player.hpp"

void GameStateMenuStatus::handleEvent(sf::Event& event)
{
}

void GameStateMenuStatus::handleInput(float dt)
{
}

void GameStateMenuStatus::update(float dt)
{
}

void GameStateMenuStatus::draw(sf::RenderWindow& window, float dt) const
{
	window.setView(this->view);

	window.draw(nameMsgBox);
	window.draw(levelMenu);
}

void GameStateMenuStatus::callbackQuit(int index)
{
	state = prevState;
}
