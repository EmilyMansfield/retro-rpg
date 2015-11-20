#ifndef PLAYER_RENDERER_HPP
#define PLAYER_RENDERER_HPP

#include "tile_set.hpp"
#include "tile_map.hpp"
#include "movement_controller.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <cmath>

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
		interp = 0.0f;

		// id of the playing animation
		std::string animString;
		if(movementController.moving)
		{
			animString = std::string("player_walk_") + (char)movementController.movementDir();
			// this->interp += dt * speed;

			// Set the sprite position depending on the direction and interpolation
			// this->sprite.setPosition((float)ts * (this->pos + this->interp * dirToVec(this->movementDir)));
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
