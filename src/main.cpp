#include <algorithm>
#include <utility>
#include <cstdlib>
#include <ctime>
#include "JsonBox.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <memory>

#include "entity_manager.hpp"
#include "game_state.hpp"
#include "game_state_title.hpp"

class TileSet;
class Item;
class Weapon;
class Armor;
class Creature;
class Door;
class Area;

// Keeps track of items, weapons, creatures etc.
EntityManager entityManager;

int main()
{
	// Load the entities
	entityManager.loadJson<TileSet>("tilesets.json");
	entityManager.loadJson<Item>("items.json");
	entityManager.loadJson<Weapon>("weapons.json");
	entityManager.loadJson<Armor>("armor.json");
	entityManager.loadJson<Creature>("creatures.json");
	entityManager.loadJson<Door>("doors.json");
	entityManager.loadJson<Area>("areas.json");

	// Seed the random number generator with the system time, so the
	// random numbers produced by rand() will be different each time
	std::srand(std::time(nullptr));

	// Current game state object
	std::shared_ptr<GameState> currentState(new GameStateTitle(currentState, &entityManager));

	// Open a window which can be closed and has a titlebar, but cannot
	// be resized. Limit the framerate to 60fps.
	sf::RenderWindow window(sf::VideoMode(512, 480), "Penguin Programmer RPG",
		sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);

	// Clock to keep track of frame timing and make the game run consistently
	// The constructor starts the clock
	sf::Clock clock;

	// Main game loop
	while(window.isOpen())
	{
		// clock::restart resets the clock to zero and starts it again,
		// but also returns the time it had accumulated. By restarting the
		// clock each frame, it will return how long the frame took, which
		// we will call dt
		float dt = clock.restart().asSeconds();

		// Event to be processed
		sf::Event event;
		// Iterate over the events triggered since the start of the last frame
		while(window.pollEvent(event))
		{
			// All game states have the close event in common, so handle
			// it here instead of in each event. Could also be placed in
			// the base GameState class
			if(event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			// Handle the event
			if(currentState) currentState->handleEvent(event);
		}
		if(currentState) currentState->handleInput(dt);

		if(!currentState)
		{
			window.close();
			break;
		}
		// Update the game proportionally to dt
		if(currentState) currentState->update(dt);

		// Render the window
		window.clear(sf::Color::Black);
		if(currentState) currentState->draw(window, dt);
		window.display();
	}

	return 0;
}
