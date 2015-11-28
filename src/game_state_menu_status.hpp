#ifndef GAME_STATE_MENU_STATUS_HPP
#define GAME_STATE_MENU_STATUS_HPP

#include <memory>

#include "game_state.hpp"
#include "player.hpp"
#include "gui/menu.hpp"
#include "gui/message_box.hpp"

class GameStateMenuStatus : public GameState
{
	private:

	std::shared_ptr<GameState>& state;
	std::shared_ptr<GameState> prevState;
	std::shared_ptr<Player> player;

	sf::View view;

	gui::MessageBox nameMsgBox;
	gui::Menu levelMenu;

	public:

	virtual void handleEvent(sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow& window, float dt) const;

	GameStateMenuStatus(std::shared_ptr<GameState>& ptr,
		std::shared_ptr<GameState> prevPtr,
		std::shared_ptr<Player> player) : state(ptr)
	{
		this->prevState = prevPtr;
		this->player = player;
		this->view.reset(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f));

		// Player name and class name
		std::string nameStr = player->name + " the " + player->className;
		nameMsgBox = gui::MessageBox(sf::Vector2u(256/8-2, 1), nameStr, mainFont);
		nameMsgBox.setPosition(sf::Vector2f(0, 0));

		// Player level and experience
		levelMenu = gui::Menu(sf::Vector2u(1, 3), sf::Vector2u(128/8-2, 1), mainFont);
		levelMenu.setPosition(sf::Vector2f(0, 8*nameMsgBox.getSize().y));
		levelMenu.addEntry("Level: " + std::to_string(player->level), nullptr);
		levelMenu.addEntry("", nullptr);
		levelMenu.addEntry("Exp:   " + std::to_string(player->xp) + " / "
			+ std::to_string(player->xpToLevel(player->level+1)), nullptr);
	}

	static void titleCallback(void* ptr, int index)
	{
		static_cast<GameStateMenuStatus*>(ptr)->callbackQuit(index);
	}
	void callbackQuit(int index);
};

#endif /* GAME_STATE_MENU_STATUS_HPP */
