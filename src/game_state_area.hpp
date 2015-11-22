#ifndef GAME_STATE_AREA_HPP
#define GAME_STATE_AREA_HPP

#include "game_state.hpp"
#include "area.hpp"
#include "tile_map.hpp"
#include "player.hpp"
#include "bitmap_font.hpp"
#include "bitmap_text.hpp"

class GameStateArea : public GameState
{
	private:

	Area* area;
	sf::View view;
	Player* player;
	BitmapFont font;
	BitmapText text;

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
		this->text = BitmapText(
			"\x80\x87\x87\x87\x87\x87\x81\n\x86Hello\x84\n\x83\x85\x85\x85\x85\x85\x82",
			this->font);
		this->text.setBackgroundColor(sf::Color(0x00, 0x40, 0x58));
		this->text.setPosition(0, 7*tm.ts);
	}
};

#endif /* GAME_STATE_AREA_HPP */
