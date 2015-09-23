#ifndef PLAYER_RENDERER_HPP
#define PLAYER_RENDERER_HPP

#include "tile_set.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <iostream>

// Direction the player is moving in
enum class Direction { NORTH = 'n', SOUTH = 's', EAST = 'e', WEST = 'w' };

class PlayerRenderer
{
	private:

	// Current interpolation value used for movement, 0 = at pos, 1 = at dest
	float interp = 0.0f;

	// Tilesheet containing the player's texture frames
	TileSet* tiles;
	unsigned int ts;

	// Position of the adjacent square that the player is trying to move to.
	// If the player is not moving, this should be the same as pos
	sf::Vector2u dest;

	sf::Vector2u dirToVec(Direction dir)
	{
		switch(dir)
		{
			case Direction::NORTH:
			return sf::Vector2u(0, -1);

			case Direction::EAST:
			return sf::Vector2u(1, 0);

			case Direction::SOUTH:
			return sf::Vector2u(0, 1);

			case Direction::WEST:
			return sf::Vector2u(-1, 0);

			default:
			return sf::Vector2u(0, 0);
		}
	}

	public:

	// Sprite used to display the player
	sf::Sprite sprite;

	// Position of the player in the game world, in grid squares not pixels
	sf::Vector2u pos;
	// Speed at which the player moves, in grid squares per second
	float speed = 1.0f;

	Direction movementDir;

	PlayerRenderer() {}
	PlayerRenderer(TileSet* tiles)
	{
		this->tiles = tiles;
		this->sprite.setTexture(this->tiles->tex);
		this->ts = this->tiles->tilesize;
		this->movementDir = Direction::SOUTH;
	}

	void update(float dt)
	{
		// id of the playing animation
		std::string animString;
		// Check if the player is idle or has finished their movement
		if(this->dest == this->pos || interp >= 1.0f)
		{
			animString = std::string("player_idle_") + (char)movementDir;
			// Reset the linear interpolation value
			this->interp = 0.0f;
			this->dest = this->pos;
		}
		else
		{
			animString = std::string("player_walk_") + (char)movementDir;
			// Increase the linear interpolation value
			// Increase by number of tiles travelled in dt
			this->interp += dt * speed;
		}
		std::cout << animString << std::endl;

		// Set the sprite position depending on the direction and interpolation
		sf::Vector2f spritePos(this->pos.x, this->pos.y);
		spritePos += sf::Vector2f(
			this->interp * dirToVec(this->movementDir).x,
			this->interp * dirToVec(this->movementDir).y
		);
		this->sprite.setPosition(spritePos * (float)ts);

		// Now calculate the animation frame based on the interpolation value
		Animation& a = this->tiles->animations[animString];
		unsigned int frame = this->interp * (float)a.len;

		// Set the texture rectangle from the frame
		this->sprite.setTextureRect(sf::IntRect(
			(a.x + frame) * ts, a.y * ts, ts, ts));
	}

	void step(Direction dir)
	{
		this->movementDir = dir;
		this->dest = this->pos + dirToVec(dir);
	}

	// Set position and stop movement
	void setPos(sf::Vector2u pos)
	{
		this->pos = pos;
		this->dest = pos;
		this->sprite.setPosition(sf::Vector2f(this->pos.x * ts, this->pos.y * ts));
	}
};

#endif /* PLAYER_RENDERER_HPP */
