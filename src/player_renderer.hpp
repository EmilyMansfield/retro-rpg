#ifndef PLAYER_RENDERER_HPP
#define PLAYER_RENDERER_HPP

#include "tile_set.hpp"
#include "tile_map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <cmath>

// Direction the player is moving in
enum class Direction { NORTH = 'n', SOUTH = 's', EAST = 'e', WEST = 'w', NONE = 0 };

class PlayerRenderer
{
	private:

	// Tilesheet containing the player's texture frames
	TileSet* tiles;
	unsigned int ts;

	Direction moveIntention;
	Direction lastMove;
	sf::Vector2f destination;
	sf::Vector2f lastPos;
	bool moving;
	float interp;
	float moveTimer;
	float moveDelay = 0.15f;
	bool moveTimerOn;

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

	Direction vecToDir(sf::Vector2f dir)
	{
		if(fabs(dir.x) > fabs(dir.y))
			return (dir.x > 0 ? Direction::EAST : Direction::WEST);
		else
			return (dir.y > 0 ? Direction::SOUTH : Direction::NORTH);
	}

	public:

	// Sprite used to display the player
	sf::Sprite sprite;

	// Position of the player in the game world, in grid squares not pixels
	sf::Vector2f pos;
	sf::Vector2f velocity;
	// Speed at which the player moves, in grid squares per second
	float speed = 3.0f;

	PlayerRenderer() {}
	PlayerRenderer(TileSet* tiles)
	{
		this->tiles = tiles;
		this->sprite.setTexture(this->tiles->tex);
		this->ts = this->tiles->tilesize;
		this->interp = 0.0f;
		this->moveIntention = Direction::NONE;
		this->lastMove = Direction::SOUTH;
		this->moving = false;
		this->moveTimer = 0.0f;
		this->moveTimerOn = false;
	}


	void setPos(sf::Vector2f pos)
	{
		this->pos = pos;
		this->sprite.setPosition((float)ts * this->pos);
	}

	void continueMovingTo()
	{
		velocity = destination - pos;
		velocity /= (float)sqrt(velocity.x*velocity.x+velocity.y*velocity.y);
		velocity *= speed;
	}
	void continueMovingFrom()
	{
		// Set new destination
		destination = destination + dirToVec(lastMove);
		// Move towards new destination
		continueMovingTo();
	}
	void changeDirection(Direction dir)
	{
		// Snap to destination tile
		setPos(destination);
		// Set new destination
		destination = destination + dirToVec(dir);
		// Move towards the new destination
		continueMovingTo();
		// Remember move
		lastMove = dir;
	}

	void startMoving(Direction dir)
	{
		// Set new destination
		destination = pos + dirToVec(dir);
		// Move
		moving = true;
		continueMovingTo();
		// Remember move
		lastMove = dir;
	}
	void stopMoving()
	{
		// Snap to destination tile to avoid inaccuracies
		setPos(destination);
		// Stop moving
		moving = false;
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}

	bool justReachedDestination()
	{
		return (pos.x >= destination.x && lastPos.x < destination.x) ||
			(pos.x <= destination.x && lastPos.x > destination.x) ||
			(pos.y >= destination.y && lastPos.y < destination.y) ||
			(pos.y <= destination.y && lastPos.y > destination.y);
	}

	bool canMoveIn(sf::Vector2f start, Direction dir, TileMap& tm)
	{
		return tm.at(start + dirToVec(dir)) == 0;
	}

	void update(float dt)
	{
		// Move player according to velocity
		lastPos = pos;
		setPos(pos + dt * velocity);
		interp = 0.0f;

		// id of the playing animation
		std::string animString;
		if(this->moving)
		{
			animString = std::string("player_walk_") + (char)vecToDir(velocity);
			// this->interp += dt * speed;

			// Set the sprite position depending on the direction and interpolation
			// this->sprite.setPosition((float)ts * (this->pos + this->interp * dirToVec(this->movementDir)));
		}
		else
		{
			animString = std::string("player_idle_") + (char)(velocity == sf::Vector2f(0,0) ? lastMove : vecToDir(velocity));
		}

		// Now calculate the animation frame based on the interpolation value
		Animation& a = this->tiles->animations[animString];
		unsigned int frame = this->interp * (float)a.len;

		// Set the texture rectangle from the frame
		this->sprite.setTextureRect(sf::IntRect((a.x + frame) * ts, a.y * ts, ts, ts));
	}

	void step(float dt, Direction dir, TileMap& tm)
	{
		// Set direction to move in
		this->moveIntention = dir;

		// Increment the move delay counter if necessary
		if(moveTimerOn) moveTimer += dt;
		// Reset timer if no move button is held
		if(moveIntention == Direction::NONE)
		{
			moveTimerOn = false;
			moveTimer = 0.0f;
		}

		// Stop if at the destination
		if(moving && justReachedDestination() && moveIntention == Direction::NONE)
		{
			stopMoving();
		}

		// Stop if at a wall
		else if(moving && justReachedDestination() && moveIntention != Direction::NONE &&
			!canMoveIn(destination, moveIntention, tm))
		{
			stopMoving();
		}

		// Destination reached but continue in the same direction
		else if(moving && justReachedDestination() && moveIntention != Direction::NONE &&
			canMoveIn(destination, moveIntention, tm) && moveIntention == lastMove)
		{
			continueMovingFrom();
		}

		// Destination reached but continue in a different direction
		else if(moving && justReachedDestination() && moveIntention != Direction::NONE &&
			canMoveIn(destination, moveIntention, tm) && moveIntention != lastMove)
		{
			changeDirection(moveIntention);
		}

		// Destination not reached so keep going
		else if(moving && !justReachedDestination())
		{
			continueMovingTo();
		}

		// Change direction from stationary, and start movement timer
		else if(!moveTimerOn && !moving && moveIntention != Direction::NONE && canMoveIn(pos, moveIntention, tm))
		{
			moveTimerOn = true;
			moveTimer = 0.0f;
			lastMove = moveIntention;
		}

		// Start moving from stationary
		else if(moveTimerOn && moveTimer >= moveDelay && !moving && moveIntention != Direction::NONE && canMoveIn(pos, moveIntention, tm))
		{
			startMoving(moveIntention);
			moveTimerOn = false;
			moveTimer = 0.0f;
		}

		// Change direction but don't move
		else if(!moving && moveIntention != Direction::NONE && !canMoveIn(pos, moveIntention, tm))
		{
			lastMove = moveIntention;
		}
	}
};

#endif /* PLAYER_RENDERER_HPP */
