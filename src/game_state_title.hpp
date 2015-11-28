#ifndef GAME_STATE_TITLE_HPP
#define GAME_STATE_TITLE_HPP

#include <memory>

#include "game_state.hpp"
#include "player.hpp"
#include "gui/menu.hpp"
#include "gui/message_box.hpp"

class EntityManager;

class GameStateTitle : public GameState
{
	private:

	enum class SubState { TITLE, NAME, CLASS };
	SubState subState;

	std::shared_ptr<GameState>& state;
	EntityManager* mgr;

	sf::View view;
	gui::Menu titleMenu;

	// Name substate
	gui::MessageBox nameEntry;
	gui::MessageBox nameEntryBox;
	std::string name;

	// Class substate
	gui::MessageBox classEntry;
	gui::Menu classEntryMenu;

	public:

	virtual void handleEvent(sf::Event& event);
	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow& window, float dt) const;

	GameStateTitle(std::shared_ptr<GameState>& ptr, EntityManager* mgr) : state(ptr)
	{
		this->mgr = mgr;
		this->view.reset(sf::FloatRect(0.0f, 0.0f, 256.0f, 240.0f));
		this->subState = SubState::TITLE;

		// Title substate
		titleMenu = gui::Menu(sf::Vector2u(1, 2), sf::Vector2u(8, 2), mainFont);
		titleMenu.addEntry("Continue", titleCallback);
		titleMenu.addEntry("Quit", titleCallback);
		titleMenu.setPosition(
			128.0f - 8*titleMenu.getSize().x/2,
			120.0f - 8*titleMenu.getSize().y/2);
		titleMenu.select(0, '*');

		// Name substate
		nameEntry = gui::MessageBox(sf::Vector2u(17, 1), "What's your name?", mainFont);
		nameEntry.setPosition(
			128.0f - 8*nameEntry.getSize().x/2,
			120.0f - 8*nameEntry.getSize().y/2);
		nameEntryBox = gui::MessageBox(sf::Vector2u(1, 1), "", mainFont);
		nameEntryBox.setPosition(
			128.0f - 8*nameEntryBox.getSize().x/2,
			120.0f + 8*nameEntry.getSize().y/2);
		
		// Class substate
		classEntry = gui::MessageBox(sf::Vector2u(18, 1), "What's your class?", mainFont);
		classEntry.setPosition(
			128.0f - 8*classEntry.getSize().x/2,
			120.0f - 8*classEntry.getSize().y/2);
		classEntryMenu = gui::Menu(sf::Vector2u(1, 2), sf::Vector2u(7, 2), mainFont);
		classEntryMenu.addEntry("Fighter", classCallback);
		classEntryMenu.addEntry("Rogue", classCallback);
		classEntryMenu.setPosition(
			128.0f - 8*classEntryMenu.getSize().x/2,
			120.0f + 8*classEntry.getSize().y/2);
		classEntryMenu.select(0, '*');
	}

	static void titleCallback(void* ptr, int index)
	{
		switch(index)
		{
			case 0: static_cast<GameStateTitle*>(ptr)->callbackContinue(index); break;
			case 1: static_cast<GameStateTitle*>(ptr)->callbackQuit(index); break;
			default: break;
		}
	}
	static void classCallback(void *ptr, int index)
	{
		static_cast<GameStateTitle*>(ptr)->callbackCreatePlayer(index);
	}
	void callbackContinue(int index);
	void callbackQuit(int index);
	void callbackCreatePlayer(int index);
};

#endif /* GAME_STATE_TITLE_HPP */
