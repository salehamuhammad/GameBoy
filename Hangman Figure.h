#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
using namespace sf;

class hangman_figure
{
private:
	Texture hangmanTextures[7];
	Sprite hangmanSprite;

public:
	hangman_figure()
	{



		for (int i = 0; i < 7; ++i)
		{
			if (!hangmanTextures[i].loadFromFile("Textures/" + to_string(i + 1) + ".png")) {
				cout << "Failed to load human figure " << (i + 1) << endl;
				exit(1);
			}

		}
	}

	Sprite& getsprite()
	{
		return hangmanSprite;
	}

	Texture* gettexture(int wrong_guess) 
	{
		
		if (wrong_guess >= 0 && wrong_guess < 7)
		{
			return &hangmanTextures[wrong_guess];
		}
		return nullptr; 
	}

	
};