#ifndef PLAYER_RENDERER_HPP
#define PLAYER_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>

#include "tile_set.hpp"
#include "tile_map.hpp"
#include "movement_controller.hpp"

class PlayerRenderer
{
	private:

	MovementController movementController;

	// Tilesheet containing the player's texture frames
	TileSet* tiles;
	unsigned int ts;

	float interp;

	public:

	// Sprite used to display the player
	sf::Sprite sprite;

	PlayerRenderer() {}
	PlayerRenderer(TileSet* tiles)
	{
		this->tiles = tiles;
		this->sprite.setTexture(this->tiles->tex);
		this->ts = this->tiles->tilesize;
		this->interp = 0.0f;
		this->movementController = MovementController(3.0f, 0.1f);
	}

	void update(float dt)
	{
		// Move player according to velocity
		movementController.update(dt);
		this->sprite.setPosition((float)ts * movementController.pos);
		// Calculate the animation interpolation value by calculating
		// how far through a single tile of movement the sprite is.
		// Note that using modf would be safer, but we assume positions
		// are smaller than 2^31, which I think is reasonable
		sf::Vector2f p1 = movementController.pos;
		sf::Vector2f p2 = sf::Vector2f((int)p1.x, (int)p1.y);
		// One of these terms is zero, so this is equivalent to taking
		// max(fabs(p1.x-p2.x), fabs(p1.y-p2.y))
		interp = fabs(p1.x-p2.x) + fabs(p1.y-p2.y);

		// id of the playing animation
		std::string animString;
		if(movementController.moving)
		{
			animString = std::string("player_walk_") + (char)movementController.movementDir();
		}
		else
		{
			animString = std::string("player_idle_") + (char)movementController.movementDir();
		}

		// Now calculate the animation frame based on the interpolation value
		Animation& a = this->tiles->animations[animString];
		unsigned int frame = this->interp * (float)a.len;

		// Set the texture rectangle from the frame
		this->sprite.setTextureRect(sf::IntRect((a.x + frame) * ts, a.y * ts, ts, ts));
	}

	void step(float dt, Direction dir, TileMap &tm)
	{
		movementController.step(dt, dir, tm);
	}

	void setPos(sf::Vector2f pos)
	{
		movementController.setPos(pos);
	}
};

#endif /* PLAYER_RENDERER_HPP */
