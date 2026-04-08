#pragma once
#include <SFML/Graphics.hpp>
#include"grid.h"
#include"Position.h"
#include<iostream>

using namespace std;
using namespace sf;

class snakesegment 
{
private:
    Position p;
    RectangleShape segmntshape;

public:
    snakesegment(int x = 0, int y = 0, int cell_size = 30)
    {
        p = Position(static_cast<float>(x * cell_size), static_cast<float>(y * cell_size));
        segmntshape.setSize({ static_cast<float>(cell_size), static_cast<float>(cell_size) });
        segmntshape.setFillColor(Color(0, 128, 0));
        segmntshape.setOutlineColor(Color(34, 139, 34));
        segmntshape.setOutlineThickness(2);
        segmntshape.setPosition(p.getx(), p.gety());
    }

    Position get_position() const 
    {
        return p; 
    }

    void set_position(int x, int y, int cellsize)//set pos of snake segs
    {
        p = Position(static_cast<float>(x * cellsize), static_cast<float>(y * cellsize));
        segmntshape.setPosition(p.getx(), p.gety());
    }

    void draw_seg(RenderWindow& wi)
    {
        wi.draw(segmntshape);
    }


};











