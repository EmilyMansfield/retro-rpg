#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_area.hpp"
#include "game_state_menu_status.hpp"

void GameStateArea::handleEvent(sf::Event& event)
{
	if(subState == SubState::GAME)
	{
		if(event.type == sf::Event::KeyPressed)
		{
			// Open the start menu
			if(event.key.code == sf::Keyboard::Return)
			{
				subState = SubState::START;
				startMenu.select(0, '*');
			}	
		}
	}
	else if(subState == SubState::START)
	{
		if(event.type == sf::Event::KeyPressed)
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
	}
	else if(subState == SubState::INFO)
	{
		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Return)
			{
				if(infoMsgBox.getPage() == infoMsgBox.numPages()-1)
					subState = SubState::GAME;
				else
					infoMsgBox.setPage(infoMsgBox.getPage()+1);
			}
		}
	}
}

void GameStateArea::handleInput(float dt)
{
	if(subState == SubState::GAME)
	{
		// Handle player movement	
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			this->player->step(dt, Direction::NORTH, this->area->tilemap);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			this->player->step(dt, Direction::EAST, this->area->tilemap);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			this->player->step(dt, Direction::SOUTH, this->area->tilemap);
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			this->player->step(dt, Direction::WEST, this->area->tilemap);
		else
			this->player->step(dt, Direction::NONE, this->area->tilemap);
	}
}

void GameStateArea::update(float dt)
{
	if(subState == SubState::GAME)
	{
		this->player->update(dt);
	}
}

void GameStateArea::draw(sf::RenderWindow& window, float dt) const
{
	window.setView(this->view);
	window.draw(this->area->tilemap);
	window.draw(*this->player->renderer);
	if(subState == SubState::START)
	{
		window.draw(startMenu);
	}
	else if(subState == SubState::INFO)
	{
		window.draw(infoMsgBox);
	}
}

void GameStateArea::callbackItems(int index)
{

}
void GameStateArea::callbackStatus(int index)
{
	std::shared_ptr<GameState> prevState = state;
	state.reset(new GameStateMenuStatus(state, prevState, player));
}
void GameStateArea::callbackSave(int index)
{
	player->save(mgr);
	infoMsgBox.setText("Saved game.");
	subState = SubState::INFO;
}
void GameStateArea::callbackExit(int index)
{
	subState = SubState::GAME;
}
