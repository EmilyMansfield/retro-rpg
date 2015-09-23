#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_area.hpp"
#include "area.hpp"

void GameStateArea::handleInput(sf::Event& event)
{

}

void GameStateArea::update(float dt)
{

}

void GameStateArea::draw(sf::RenderWindow& window, float dt) const
{
	window.setView(this->view);
	window.draw(this->area->tilemap);
}
