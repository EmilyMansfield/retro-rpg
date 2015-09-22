#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

class GameState
{
	public:

	virtual void draw() = 0;
	virtual void update() = 0;
	virtual void handleInput() = 0;

	virtual ~GameState() {}	
};

#endif /* GAME_STATE_HPP */
