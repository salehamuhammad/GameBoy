#pragma once
class Position
{
    
    float x, y;

public:


    Position(float x = 0, float y = 0) : x(x), y(y)
    {}

    Position operator+(const Position& ot) const
    {
        return Position(x + ot.x, y + ot.y);
    }

    Position operator*(float s) const
    {
        return Position(x * s, y * s);
    }

    bool operator==(const Position& ot) const
    {
        return x == ot.x && y == ot.y;

    }

    float getx()const
    {
        return x;
    }

    float gety()const
    {
        return y;
    }

    void setx(float new_x) 
    
    {
        this->x = new_x;
    
    } 
    void sety(float new_y)
    {
        
        this->y = new_y;
    }
};
