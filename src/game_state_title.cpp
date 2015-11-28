#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include "JsonBox.h"

#include "game_state_title.hpp"
#include "player_renderer.hpp"
#include "player.hpp"
#include "entity_manager.hpp"
#include "game_state_area.hpp"

void GameStateTitle::handleEvent(sf::Event& event)
{
	if(subState == SubState::TITLE)
	{
		// Handle menu navigation
		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Up)
				titleMenu.navigate(gui::Direction::UP, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
			else if(event.key.code == sf::Keyboard::Down)
				titleMenu.navigate(gui::Direction::DOWN, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
			// Open a menu option
			else if(event.key.code == sf::Keyboard::Return)
				titleMenu.activate(this);
		}
	}
	else if(subState == SubState::NAME)
	{
		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Return)
			{
				std::ifstream f((name + ".json").c_str());
				if(f.good())
				{
					f.close();
					// Load the player
					JsonBox::Value saveData;
					JsonBox::Value areaData;
					saveData.loadFromFile(name + ".json");
					areaData.loadFromFile(name + "_areas.json");
					std::shared_ptr<Player> player(new Player(saveData, areaData, mgr));
					player->renderer = PlayerRenderer(mgr->getEntity<TileSet>("tileset_overworld"));
					player->renderer.setPos(sf::Vector2f(2, 2));
					player->currentArea = "area_01";
					player->visitedAreas.insert(player->currentArea);
					state.reset(new GameStateArea(state, player, mgr));
				}
				else
				{
					f.close();
					subState = SubState::CLASS;
				}
			}
		}
		else if(event.type == sf::Event::TextEntered)
		{
			if(event.text.unicode < 127)
			{
				char c = static_cast<char>(event.text.unicode);
				if(c == '\b' && name.size() > 0) name.pop_back();
				else if(c >= ' ' && name.size() < (256/8-2)) name.push_back(c);
				nameEntryBox = gui::MessageBox(sf::Vector2u(name.size(), 1), name, mainFont);
				nameEntryBox.setPosition(sf::Vector2f(
					128.0f - 8*nameEntryBox.getSize().x/2,
					120.0 + 8*nameEntry.getSize().y/2));
			}
		}
	}
	else if(subState == SubState::CLASS)
	{
		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::Up)
				classEntryMenu.navigate(gui::Direction::UP, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
			else if(event.key.code == sf::Keyboard::Down)
				classEntryMenu.navigate(gui::Direction::DOWN, gui::NavigationMode::STOP, gui::NavigationMode::LOOP);
			else if(event.key.code == sf::Keyboard::Return)
				classEntryMenu.activate(this);
		}
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

	if(subState == SubState::TITLE)
		window.draw(titleMenu);
	else if(subState == SubState::NAME)
	{
		window.draw(nameEntry);
		window.draw(nameEntryBox);
	}
	else if(subState == SubState::CLASS)
	{
		window.draw(classEntry);
		window.draw(classEntryMenu);
	}
}

void GameStateTitle::callbackContinue(int index)
{
	subState = SubState::NAME;

}
void GameStateTitle::callbackQuit(int index)
{
	state.reset();
}
void GameStateTitle::callbackCreatePlayer(int index)
{
	std::shared_ptr<Player> player;
	switch(index)
	{
		case 0:
			player.reset(new Player(name, 15, 5, 4, 1.0/64.0, 0, 1, "Fighter"));
			break;
		case 1:
			player.reset(new Player(name, 15, 4, 5, 1.0/64.0, 0, 1, "Rogue"));
			break;
		default:
			player.reset(new Player(name, 15, 4, 4, 1.0/64.0, 0, 1, "Adventurer"));
	}
	player->renderer = PlayerRenderer(mgr->getEntity<TileSet>("tileset_overworld"));
	player->renderer.setPos(sf::Vector2f(2, 2));
	player->currentArea = "area_01";
	player->visitedAreas.insert(player->currentArea);
	state.reset(new GameStateArea(state, player, mgr));
}
