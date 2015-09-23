#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_area.hpp"
#include "area.hpp"

void GameStateArea::handleInput(sf::Event& event)
{

}

void GameStateArea::update(float dt)
{
	this->player->renderer.update(dt);
}

void GameStateArea::draw(sf::RenderWindow& window, float dt) const
{
	window.setView(this->view);
	window.draw(this->area->tilemap);
	window.draw(this->player->renderer.sprite);
}
