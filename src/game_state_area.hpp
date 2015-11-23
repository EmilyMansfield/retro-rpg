#ifndef GAME_STATE_AREA_HPP
#define GAME_STATE_AREA_HPP

#include "game_state.hpp"
#include "area.hpp"
#include "tile_map.hpp"
#include "player.hpp"
#include "gui/font.hpp"
#include "gui/text.hpp"
#include "gui/message_box.hpp"

class GameStateArea : public GameState
{
	private:

	Area* area;
	sf::View view;
	Player* player;
	gui::Font font;
	gui::MessageBox msgBox;

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

		this->font.loadFromFile("assets/font.png");
		this->msgBox = gui::MessageBox(
			sf::IntRect(0, 0, 32, 5),
			"Hello, world!\nHow's it going, are you done yet?",
			this->font);
		this->msgBox.setPosition(0, 7*tm.ts);
	}
};

#endif /* GAME_STATE_AREA_HPP */
