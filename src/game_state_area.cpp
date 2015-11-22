#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_area.hpp"
#include "area.hpp"
#include "player_renderer.hpp"

void GameStateArea::handleEvent(sf::Event& event)
{

}

void GameStateArea::handleInput(float dt)
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		this->player->renderer.step(dt, Direction::NORTH, this->area->tilemap);
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		this->player->renderer.step(dt, Direction::EAST, this->area->tilemap);
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		this->player->renderer.step(dt, Direction::SOUTH, this->area->tilemap);
	else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		this->player->renderer.step(dt, Direction::WEST, this->area->tilemap);
	else
		this->player->renderer.step(dt, Direction::NONE, this->area->tilemap);
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
	window.draw(this->text);
}
