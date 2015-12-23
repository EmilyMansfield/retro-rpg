#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_menu_items.hpp"
#include "player.hpp"

void GameStateMenuItems::handleEvent(sf::Event& event)
{
	if(event.type == sf::Event::KeyPressed)
	{
		// Return to the game
		if(event.key.code == sf::Keyboard::Escape)
		{
			callbackQuit(0);
		}
		// Handle menu navigation
		else if(event.key.code == sf::Keyboard::Up)
		{
			itemMenu.navigate(gui::Direction::UP, gui::NavigationMode::STOP, gui::NavigationMode::PAGE);
			updateInfo();
		}
		else if(event.key.code == sf::Keyboard::Down)
		{
			itemMenu.navigate(gui::Direction::DOWN, gui::NavigationMode::STOP, gui::NavigationMode::PAGE);
			updateInfo();
		}
		// Open a menu option
		else if(event.key.code == sf::Keyboard::Return)
			itemMenu.activate(this);
	}
}

void GameStateMenuItems::handleInput(float dt)
{
}

void GameStateMenuItems::update(float dt)
{
}

void GameStateMenuItems::draw(sf::RenderWindow& window, float dt) const
{
	window.setView(this->view);

	window.draw(titleMsgBox);
	window.draw(itemMenu);
	window.draw(infoMsgBox);
}

void GameStateMenuItems::callbackQuit(size_t index)
{
	state = prevState;
}

void GameStateMenuItems::updateInfo()
{
	size_t selected = itemMenu.getSelected();
	if(selected < player->inventory.size())
	{
		Item* item = player->inventory.get(selected);
		infoMsgBox.setText(item->description);
	}
	else
	{
		infoMsgBox.setText("");
	}
}
