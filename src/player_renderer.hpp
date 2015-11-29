#ifndef PLAYER_RENDERER_HPP
#define PLAYER_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <cmath>

#include "tile_set.hpp"
#include "tile_map.hpp"
#include "creature_mover.hpp"

class PlayerRenderer
{
	private:

	CreatureMover mover;

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
		this->mover = CreatureMover(3.0f, 0.1f);
	}

	void update(float dt)
	{
		// Move player according to velocity
		mover.update(dt);
		this->sprite.setPosition((float)ts * mover.getPosition());
		// Calculate the animation interpolation value by calculating
		// how far through a single tile of movement the sprite is.
		// Note that using modf would be safer, but we assume positions
		// are smaller than 2^31, which I think is reasonable
		sf::Vector2f p1 = mover.getPosition();
		sf::Vector2f p2 = sf::Vector2f((int)p1.x, (int)p1.y);
		// One of these terms is zero, so this is equivalent to taking
		// max(fabs(p1.x-p2.x), fabs(p1.y-p2.y))
		interp = fabs(p1.x-p2.x) + fabs(p1.y-p2.y);

		// id of the playing animation
		std::string animString;
		if(mover.isMoving())
		{
			animString = std::string("player_walk_") + (char)mover.getFacing();
		}
		else
		{
			animString = std::string("player_idle_") + (char)mover.getFacing();
		}

		// Now calculate the animation frame based on the interpolation value
		Animation& a = this->tiles->animations[animString];
		unsigned int frame = this->interp * (float)a.len;

		// Set the texture rectangle from the frame
		this->sprite.setTextureRect(sf::IntRect((a.x + frame) * ts, a.y * ts, ts, ts));
	}

	void step(float dt, Direction dir, TileMap &tm)
	{
		mover.step(dt, dir, tm);
	}

	void setPosition(sf::Vector2f pos)
	{
		mover.setPosition(pos);
	}
};

#endif /* PLAYER_RENDERER_HPP */
