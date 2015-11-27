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
		startMenu.addEntry("Items", callbackFunc);
		startMenu.addEntry("Status", callbackFunc);
		startMenu.addEntry("Save", callbackFunc);
		startMenu.addEntry("Exit", callbackFunc);
		startMenu.setPosition(sf::Vector2f(256.0f - 8*startMenu.getSize().x, 0));
		startMenuVisible = false;
	}

	// C++ distinguishes between pointers to member functions and pointers to
	// functions, and so the direct way of calling a member function from a
	// gui::Menu is by defining the callbacks to be of type void
	// (GameState::*)(int) instead of void (*)(int). We don't want to do this,
	// because we want gui::Menu to be useable without a class named GameState.
	// Luckily for us, static member functions are treated as functions and so
	// can be called via a void (*)(int) function pointer. This means that we
	// can register a static function as a callback, but let that static
	// function accept an additional void pointer as an argument, which it can
	// convert to a pointer to the GameStateArea class. If we pass a `this`
	// pointer to the static callback, it can be used to call a member function.
	// By using the index argument of the original gui::Menu callback, we can
	// use a single static callback to call an arbitrary number of member
	// callback functions.
	static void callbackFunc(void* ptr, int index)
	{
		// The alternative is to create an array of callbacks, then you can use
		// the index to call the callback directly, but this greatly complicates
		// the code, and isn't worth it for us. This is far clearer, and will do
		// just fine for our purposes
		// https://isocpp.org/wiki/faq/pointers-to-members
		switch(index)
		{
			case 0: ((GameStateArea*)ptr)->callbackItems(index); break;
			case 1: ((GameStateArea*)ptr)->callbackStatus(index); break;
			case 2: ((GameStateArea*)ptr)->callbackSave(index); break;
			case 3: ((GameStateArea*)ptr)->callbackExit(index); break;
			default: break;
		}
	}
	void callbackItems(int index);
	void callbackStatus(int index);
	void callbackSave(int index);
	void callbackExit(int index);
};

#endif /* GAME_STATE_AREA_HPP */
