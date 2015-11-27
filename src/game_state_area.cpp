#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_area.hpp"
#include "area.hpp"
#include "player_renderer.hpp"

void GameStateArea::handleEvent(sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		if(startMenuVisible)
		{
			// Handle menu navigation
			if(event.key.code == sf::Keyboard::Up)
				startMenu.navigate(gui::Direction::UP, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
			else if(event.key.code == sf::Keyboard::Right)
				startMenu.navigate(gui::Direction::RIGHT, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
			else if(event.key.code == sf::Keyboard::Down)
				startMenu.navigate(gui::Direction::DOWN, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
			else if(event.key.code == sf::Keyboard::Left)
				startMenu.navigate(gui::Direction::LEFT, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
			// Open a menu option
			else if(event.key.code == sf::Keyboard::Return)
				startMenu.activate(this);
		}
		else
		{
			// Open the start menu
			if(event.key.code == sf::Keyboard::Return)
			{
				startMenuVisible = true;
				startMenu.select(0, '*');
			}	
		}
	}
}

void GameStateArea::handleInput(float dt)
{
	if(!startMenuVisible)
	{
		// Handle player movement	
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
}

void GameStateArea::update(float dt)
{
	if(!startMenuVisible)
	{
		this->player->renderer.update(dt);
	}
}

void GameStateArea::draw(sf::RenderWindow& window, float dt) const
{
	window.setView(this->view);
	window.draw(this->area->tilemap);
	window.draw(this->player->renderer.sprite);
	if(startMenuVisible) window.draw(startMenu);
}

void GameStateArea::callbackItems(int index)
{

}
void GameStateArea::callbackStatus(int index)
{

}
void GameStateArea::callbackSave(int index)
{

}
void GameStateArea::callbackExit(int index)
{
	startMenuVisible = false;
}
