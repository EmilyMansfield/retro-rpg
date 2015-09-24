#ifndef PLAYER_RENDERER_HPP
#define PLAYER_RENDERER_HPP

#include "tile_set.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>

// Direction the player is moving in
enum class Direction { NORTH = 'n', SOUTH = 's', EAST = 'e', WEST = 'w' };

class PlayerRenderer
{
	private:

	// Current interpolation value used for movement, 0 = at pos, 1 = at dest
	float interp = 0.0f;
	// Delay in seconds before walking starts when the button is held
	float walkingDelay = 0.1f;
	float walkingTimer;

	// Tilesheet containing the player's texture frames
	TileSet* tiles;
	unsigned int ts;

	bool walking;
	bool stopping;

	sf::Vector2f dirToVec(Direction dir)
	{
		switch(dir)
		{
			case Direction::NORTH:
			return sf::Vector2f(0, -1);

			case Direction::EAST:
			return sf::Vector2f(1, 0);

			case Direction::SOUTH:
			return sf::Vector2f(0, 1);

			case Direction::WEST:
			return sf::Vector2f(-1, 0);

			default:
			return sf::Vector2f(0, 0);
		}
	}

	public:

	// Sprite used to display the player
	sf::Sprite sprite;

	// Position of the player in the game world, in grid squares not pixels
	sf::Vector2f pos;
	// Speed at which the player moves, in grid squares per second
	float speed = 3.0f;

	Direction movementDir;

	PlayerRenderer() {}
	PlayerRenderer(TileSet* tiles)
	{
		this->tiles = tiles;
		this->sprite.setTexture(this->tiles->tex);
		this->ts = this->tiles->tilesize;
		this->movementDir = Direction::SOUTH;
		this->walking = false;
		this->stopping = false;
		this->walkingTimer = 0.0f;
	}

	void update(float dt)
	{
		// id of the playing animation
		std::string animString;
		if(this->walking)
		{
			animString = std::string("player_walk_") + (char)movementDir;
			this->interp += dt * speed;

			// Set the sprite position depending on the direction and interpolation
			this->sprite.setPosition((float)ts * (this->pos + this->interp * dirToVec(this->movementDir)));	
		}
		else
		{
			animString = std::string("player_idle_") + (char)movementDir;
		}

		// Now calculate the animation frame based on the interpolation value
		Animation& a = this->tiles->animations[animString];
		unsigned int frame = this->interp * (float)a.len;

		// Set the texture rectangle from the frame
		this->sprite.setTextureRect(sf::IntRect((a.x + frame) * ts, a.y * ts, ts, ts));

		if(this->interp >= 1.0f)
		{
			if(this->stopping)
			{
				this->walking = false;
				this->stopping = false;
			}
			// this->walking = false;
			this->interp = 0.0f;
			this->setPos(this->pos + dirToVec(this->movementDir));
		}
	}

	// If the player is stationary, they can change direction by pressing
	// a direction button. Holding that button will start to move the player
	// after a delay, in which case changing direction will keep the player
	// moving
	void step(float dt, Direction dir)
	{
		// Player is not stopping if they are moving
		this->stopping = false;

		// Player is stationary and changing direction
		if(this->movementDir != dir && !this->walking)
		{
			this->movementDir = dir;
			this->walkingTimer = 0.0f;
		}
		// Player is stationary and trying to move
		else if(this->movementDir == dir && !this->walking)
		{
			if(this->walkingTimer < this->walkingDelay)
			{
				this->walkingTimer += dt;
			}
			else
			{
				this->walking = true;
				this->walkingTimer = 0.0f;
			}
		}
		// Player is moving so can change direction at the end of a motion
		else if(this->interp == 0.0f)
		{
			this->movementDir = dir;
		}
	}

	void stop()
	{
		this->stopping = true;
	}

	// Set position and stop movement
	void setPos(sf::Vector2f pos)
	{
		this->pos = pos;
		this->sprite.setPosition((float)ts * this->pos);
	}
};

#endif /* PLAYER_RENDERER_HPP */
