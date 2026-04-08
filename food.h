#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include"grid.h"
#include"Position.h"

using namespace sf;
using namespace std;

class Food 
{
private:
    int grdw, grdh, cellsize;
    Position pos;
    Texture foodTexture;
    Sprite foodSprite;

public:
    Food(int gw, int gh, int cs) : grdw(gw), grdh(gh), cellsize(cs)
    {
        if (!foodTexture.loadFromFile("Textures/food.png")) 
        {
            cout << "Failed to load food texture!" << endl;
            exit(1);
        }

        foodSprite.setTexture(foodTexture);
        foodSprite.setScale(static_cast<float>(cellsize) / static_cast<float>(foodTexture.getSize().x),
            static_cast<float>(cellsize) / static_cast<float>(foodTexture.getSize().y));

        srand(static_cast<unsigned>(time(nullptr)));
        create_new_pos(nullptr, 0); 
    }

    void create_new_pos(snakesegment  snakeBody[], int snakeLength) 
    {
        bool validPosition;
        do {
            validPosition = true;

            pos.setx(static_cast<float>(rand() % grdw) * cellsize) ;
            pos.sety(static_cast<float>(rand() % grdh) * cellsize) ;

            for (int i = 0; i < snakeLength; ++i) 
            {
                if (pos.getx() == snakeBody[i].get_position().getx() &&
                    pos.gety() == snakeBody[i].get_position().gety()) 
                {
                    validPosition = false;
                    break;
                }
            }
        } while (!validPosition);

        foodSprite.setPosition(pos.getx(), pos.gety());
    }


    Position get_position() const 
    {
        return Position(static_cast<int>(pos.getx() / cellsize), static_cast<int>(pos.gety() / cellsize));
    }

    FloatRect get_bounds() const 
    {
        return foodSprite.getGlobalBounds();
    }

    void draw_f(RenderWindow& win) 
    {
        win.draw(foodSprite);
    }
};



