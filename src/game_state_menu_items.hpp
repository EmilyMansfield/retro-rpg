#ifndef GAME_STATE_MENU_ITEMS_HPP
#define GAME_STATE_MENU_ITEMS_HPP

#include <memory>

#include "game_state.hpp"
#include "player.hpp"
#include "item.hpp"
#include "gui/menu.hpp"
#include "gui/message_box.hpp"

class GameStateMenuItems : public GameState
{
	private:

	std::shared_ptr<GameState>& state;
	std::shared_ptr<GameState> prevState;
	std::shared_ptr<Player> player;

	sf::View view;

	gui::Menu itemMenu;
	gui::MessageBox titleMsgBox;

	public:

	virtual void handleEvent(sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow& window, float dt) const;

	GameStateMenuItems(std::shared_ptr<GameState>& ptr,
		std::shared_ptr<GameState> prevPtr,
		std::shared_ptr<Player> player) :
		state(ptr),
		prevState(prevPtr),
		player(player)
	{
		this->view.reset(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f));

		titleMsgBox = gui::MessageBox(sf::Vector2u(256/8-2, 1), "Items", mainFont);
		titleMsgBox.setPosition(0, 0);

		itemMenu = gui::Menu(sf::Vector2u(1, 6), sf::Vector2u(256/8-4, 2), mainFont);
		itemMenu.setPosition(0, 8*titleMsgBox.getSize().y);
		itemMenu.setTrim(true);
		for(size_t i = 0; i < player->inventory.size(); ++i)
		{
			Item* item = player->inventory.get(i);
			if(item != nullptr)
			{
				unsigned int c = player->inventory.count(item);
				std::string countStr = std::string("x") + (c > 9 ? "" : " ") + std::to_string(c);
				size_t n = 256/8 - 4 - item->name.size() - countStr.size();
				itemMenu.addEntry(item->name + std::string(n, ' ') + countStr, itemCallback);
			}
		}
		if(player->inventory.size() > 0) itemMenu.select(0, '*');
	}

	static void itemCallback(void* ptr, int index)
	{

	}
	void callbackQuit(int index);
};

#endif /* GAME_STATE_MENU_ITEMS_HPP */
