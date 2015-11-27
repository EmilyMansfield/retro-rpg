#ifndef GAME_STATE_AREA_HPP
#define GAME_STATE_AREA_HPP

#include "game_state.hpp"
#include "area.hpp"
#include "tile_map.hpp"
#include "player.hpp"
#include "gui/font.hpp"
#include "gui/message_box.hpp"
#include "gui/menu.hpp"

class GameStateArea : public GameState
{
	private:

	Area* area;
	sf::View view;
	Player* player;

	gui::Menu startMenu;
	bool startMenuVisible;

	public:

	virtual void handleEvent(sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow& window, float dt) const;

	GameStateArea(Area* area, Player* player)
	{
		this->area = area;
		this->player = player;
		TileMap& tm = this->area->tilemap;
		// Move the tilemap origin to its centre
		tm.setOrigin(tm.ts * tm.w * 0.5, tm.ts * tm.h * 0.5);
		// Move it back so its centre is at (0,0)
		tm.setPosition(tm.ts * tm.w * 0.5, tm.ts * tm.h * 0.5);
		// Create a view of the game world
		this->view.reset(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f));

		// Set up the start menu
		startMenu = gui::Menu(sf::Vector2u(1, 4), sf::Vector2u(8, 2), mainFont);
		startMenu.addEntry("Items", nullptr);
		startMenu.addEntry("Status", nullptr);
		startMenu.addEntry("Save", nullptr);
		startMenu.addEntry("Exit", nullptr);
		startMenu.setPosition(sf::Vector2f(256.0f - 8*startMenu.getSize().x, 0));
		startMenuVisible = false;
	}
};

#endif /* GAME_STATE_AREA_HPP */
