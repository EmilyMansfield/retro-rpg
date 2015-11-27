#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "game_state_title.hpp"
#include "player_renderer.hpp"
#include "player.hpp"
#include "entity_manager.hpp"
#include "game_state_area.hpp"

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
	std::shared_ptr<Player> player(new Player("test", 10, 4, 4, 0, 0, 1, "none"));
	player->renderer = PlayerRenderer(mgr->getEntity<TileSet>("tileset_overworld"));
	player->renderer.setPos(sf::Vector2f(2, 2));
	player->currentArea = "area_01";
	player->visitedAreas.insert(player->currentArea);
	state.reset(new GameStateArea(state, player->getAreaPtr(mgr), player));
}
void GameStateTitle::callbackQuit(int index)
{
	state.reset();
}
