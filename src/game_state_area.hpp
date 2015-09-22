#ifndef GAME_STATE_AREA_HPP
#define GAME_STATE_AREA_HPP

#include "game_state.hpp"
#include "area.hpp"

class GameStateArea : public GameState
{
	private:

	Area* area;

	public:

	GameStateArea(Area* area)
	{
		this->area = area;
	}
};

#endif /* GAME_STATE_AREA_HPP */
