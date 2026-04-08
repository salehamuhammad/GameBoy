#pragma once
#include "SFML/Graphics/RenderWindow.hpp"
using namespace sf;
class game
{
private:

	int score;
public:
	virtual void startgame() = 0;
	virtual void endgame() = 0;
	virtual void render(RenderWindow& wind) = 0;
	int getscore()
	{
		return score;
	}

	void setscore(int s)
	{
		score = s;
	}
	virtual ~game() = default;
};