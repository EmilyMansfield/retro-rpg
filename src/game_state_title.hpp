#ifndef GAME_STATE_TITLE_HPP
#define GAME_STATE_TITLE_HPP

#include <memory>

#include "game_state.hpp"
#include "player.hpp"
#include "gui/menu.hpp"

class EntityManager;

class GameStateTitle : public GameState
{
	private:

	std::shared_ptr<GameState>& state;
	EntityManager* mgr;

	sf::View view;
	gui::Menu titleMenu;

	public:

	virtual void handleEvent(sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow& window, float dt) const;

	GameStateTitle(std::shared_ptr<GameState>& ptr, EntityManager* mgr) : state(ptr)
	{
		this->mgr = mgr;
		this->view.reset(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f));

		titleMenu = gui::Menu(sf::Vector2u(1, 2), sf::Vector2u(8, 2), mainFont);
		titleMenu.addEntry("Continue", callbackFunc);
		titleMenu.addEntry("Quit", callbackFunc);
		titleMenu.setPosition(sf::Vector2f(
			128.0f - 8*titleMenu.getSize().x/2,
			120 - 8*titleMenu.getSize().y/2));
		titleMenu.select(0, '*');
	}

	static void callbackFunc(void* ptr, int index)
	{
		switch(index)
		{
			case 0: ((GameStateTitle*)ptr)->callbackContinue(index); break;
			case 1: ((GameStateTitle*)ptr)->callbackQuit(index); break;
			default: break;
		}
	}
	void callbackContinue(int index);
	void callbackQuit(int index);
};

#endif /* GAME_STATE_TITLE_HPP */
