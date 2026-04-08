#pragma once
#include <SFML/Graphics.hpp>
#include<iostream>


using namespace std;
using namespace sf;


class Grid
{
private:
	int width;
	int height;
	int cell_size;
	Texture backgroundTexture;
	Sprite backgroundSprite;

public:
	Grid(int w, int h, int size) : width(w), height(h), cell_size(size)
	{
		if (!backgroundTexture.loadFromFile("Textures/b.jpg")) //for the sprite
		{
			cout << "Failed to load grid texture!" << endl;
			exit(1);
		}
		backgroundSprite.setTexture(backgroundTexture);
		
	}

	void draw_g(RenderWindow& wi) 
	{
		backgroundSprite.setColor(Color(250, 250, 250, 130));
		wi.draw(backgroundSprite); 
	}

	int get_cell_size() const
	{
		return cell_size;
	}
	int get_grd_w() const
	{ 
		return width; 
	}
	int get_grd_h() const
	{
		return height; 
	}
};