#pragma once
#include<iostream>
#include <fstream>
#include<SFML/Graphics.hpp>
#include<SFML/audio.hpp>
#include "Snake segment.h"
#include "grid.h"
#include "food.h"
#include"position.h"
#include"Game.h"


using namespace sf;
using namespace std;

class snake_game : public game
{
private:
    Position move;
    Grid grid;
    snakesegment  s_body[100];
    Food f;
    int snk_l = 3;
    bool gameover;
    bool is_paused;
    Texture gameOverTexture;
    Sprite gameOverSprite;
    Sprite resSprite;
    Texture resTexture;
    Font f1;
    Text scr_t;
    Text pause_t;
    Text scrval_t;

    SoundBuffer eatSoundBuffer;
    Sound eatSound;
    SoundBuffer gameOverSoundBuffer;
    Sound gameOverSound;
    Music backgroundMusic;
    Text restartMessage;
    Clock restartMessageClock; // To control how long the message is displayed
    bool showRestartMessage = false;



public:
    snake_game(int gw, int gh, int cell_size)
        : grid(gw, gh, cell_size), f(gw, gh, cell_size) {
        s_body[0] = snakesegment(gw / 2, gh / 2, cell_size);
        s_body[1] = snakesegment((gw / 2) - 1, gh / 2, cell_size);
        s_body[2] = snakesegment((gw / 2) - 2, gh / 2, cell_size);

        f.create_new_pos(s_body, snk_l);
        move = Position(1, 0);
        gameover = false;
        is_paused = false;

        setscore(0);
        update_score_text();
        if (!gameOverTexture.loadFromFile("Textures/GameOver.jpg"))
        {
            cout << "Failed to load game over texture!" << endl;
            exit(1);
        }

        gameOverSprite.setTexture(gameOverTexture);
        gameOverSprite.setScale(static_cast<float>(gw * cell_size) / gameOverTexture.getSize().x,
            static_cast<float>(gh * cell_size) / gameOverTexture.getSize().y);

        if (!f1.loadFromFile("Fonts/joystix monospace.otf"))
        {
            cout << "Failed to load font!" << endl;
            exit(1);
        }

        scr_t.setFont(f1);
        scr_t.setCharacterSize(30);
        scr_t.setFillColor(Color::Black);
        scr_t.setString("Score");
        scr_t.setPosition(grid.get_grd_w() * grid.get_cell_size() - 200, 10);

        scrval_t.setFont(f1);
        scrval_t.setCharacterSize(40);
        scrval_t.setFillColor(Color::Black);
        scrval_t.setPosition(scr_t.getPosition().x + 50, scr_t.getPosition().y + 40);

        update_score_text();

        pause_t.setFont(f1);
        pause_t.setCharacterSize(40);
        pause_t.setFillColor(Color::Black);
        pause_t.setString("Game Paused");
        pause_t.setPosition(grid.get_grd_w() * grid.get_cell_size() / 2 - 150,
            grid.get_grd_h() * grid.get_cell_size() / 2 - 50);

        if (!resTexture.loadFromFile("Textures/r.png")) {
            cout << "Failed to load restart button texture!" << endl;
            exit(1);
        }

        resSprite.setTexture(resTexture);
        resSprite.setScale(0.2f, 0.2f);
        resSprite.setPosition(scr_t.getPosition().x + 140, scr_t.getPosition().y);

        restartMessage.setFont(f1);
        restartMessage.setCharacterSize(40);
        restartMessage.setFillColor(Color::Black);
        restartMessage.setString("Game Restarted");
        restartMessage.setPosition(grid.get_grd_w() * grid.get_cell_size() / 2 - 180,
            grid.get_grd_h() * grid.get_cell_size() / 2 - 100);

        //sounds
        if (!eatSoundBuffer.loadFromFile("Sound Effects/growsbigger.wav")) {
            cout << "Failed to load eat sound!" << endl;
            exit(1);
        }
        eatSound.setBuffer(eatSoundBuffer);

        if (!gameOverSoundBuffer.loadFromFile("Sound Effects/death.wav"))
        {
            cout << "Failed to load game over sound!" << endl;
            exit(1);
        }
        gameOverSound.setBuffer(gameOverSoundBuffer);

        // background music
        if (!backgroundMusic.openFromFile("Sound Effects/Candies n Curses.ogg.opus"))
        {
            cout << "Failed to load background music!" << endl;
            exit(1);
        }
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(40);  //background music volume 

        backgroundMusic.play();  // background music
    }

    void reset_game()
    {

        save_score();//for saving score jab game restart ho
        snk_l = 3;
        gameover = false;
        is_paused = false;

        setscore(0);
        update_score_text();

        s_body[0] = snakesegment(grid.get_grd_w() / 2, grid.get_grd_h() / 2, grid.get_cell_size());
        s_body[1] = snakesegment((grid.get_grd_w() / 2) - 1, grid.get_grd_h() / 2, grid.get_cell_size());
        s_body[2] = snakesegment((grid.get_grd_w() / 2) - 2, grid.get_grd_h() / 2, grid.get_cell_size());

        f.create_new_pos(s_body, snk_l);
        move = Position(1, 0);

        // restart message
        showRestartMessage = true;
        restartMessageClock.restart(); //  timer for the message 
    }

    void update_score_text()
    {
        scrval_t.setString(to_string(getscore()));
    }

    void switch_pause()
    {
        is_paused = !is_paused;
    }


    void keyboard_input(RenderWindow& wind)
    {
        Event ev;
        while (wind.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
                wind.close();

            if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left)//mouse
            {
                Vector2i mousePos = Mouse::getPosition(wind);


                if (resSprite.getGlobalBounds().contains(mousePos.x, mousePos.y))//for resttart
                {
                    reset_game();
                }
            }


            if (ev.type == Event::KeyPressed)// Keyboard Inputs
            {
                if (ev.key.code == Keyboard::P)
                    switch_pause();


                if (!is_paused)// for movement  
                {
                    if (ev.key.code == Keyboard::Up && !(move.getx() == 0 && move.gety() == 1))
                        move = Position(0, -1);
                    else if (ev.key.code == Keyboard::Down && !(move.getx() == 0 && move.gety() == -1))
                        move = Position(0, 1);
                    else if (ev.key.code == Keyboard::Left && !(move.getx() == 1 && move.gety() == 0))
                        move = Position(-1, 0);
                    else if (ev.key.code == Keyboard::Right && !(move.getx() == -1 && move.gety() == 0))
                        move = Position(1, 0);
                }
            }
        }
    }



    bool collision_check()
    {
        Position head = s_body[0].get_position();
        for (int i = 1; i < snk_l; ++i)
        {
            if (head == s_body[i].get_position()) return true;
        }
        return false;
    }


    void movement_snake()
    {
        if (gameover || is_paused)//ret if the game is paused or ended
            return;


        Position nw_hdpos = s_body[0].get_position() + (move * grid.get_cell_size());//new head

        if (nw_hdpos.getx() < 0 || nw_hdpos.gety() < 0 ||
            nw_hdpos.getx() >= grid.get_grd_w() * grid.get_cell_size() ||
            nw_hdpos.gety() >= grid.get_grd_h() * grid.get_cell_size())//collision with walls
        {
            gameover = true;
            return;
        }


        if (nw_hdpos == Position(f.get_position().getx() * grid.get_cell_size(), f.get_position().gety() * grid.get_cell_size()))// collision with food
        {

            Position tailPos = s_body[snk_l - 1].get_position();  //for new segment
            s_body[snk_l] = snakesegment(
                static_cast<int>(tailPos.getx() / grid.get_cell_size()),
                static_cast<int>(tailPos.gety() / grid.get_cell_size()),
                grid.get_cell_size());
            snk_l++;

            f.create_new_pos(s_body, snk_l);
            setscore(getscore() + 1); //inc score
            update_score_text();

            // Play the eat sound
            eatSound.play();
            eatSound.setVolume(60);
        }


        for (int i = snk_l - 1; i > 0; --i)// movemnt of the snake
        {
            s_body[i] = s_body[i - 1];
        }


        s_body[0].set_position(
            static_cast<int>(nw_hdpos.getx() / grid.get_cell_size()),
            static_cast<int>(nw_hdpos.gety() / grid.get_cell_size()),
            grid.get_cell_size());// moves the head to the new position


        if (collision_check())// sees collision with itself
        {
            gameover = true;
        }

    }


    void draw_sprites(RenderWindow& wind)
    {
        if (gameover)
        {
            wind.draw(gameOverSprite);
        }
        else
        {
            for (int i = 0; i < snk_l; ++i)
                s_body[i].draw_seg(wind);
            f.draw_f(wind);


            wind.draw(scr_t);// score
            wind.draw(scrval_t);// score value


            wind.draw(resSprite); // restart button

            if (is_paused)
            {
                wind.draw(pause_t);//pause
            }

            if (showRestartMessage)
            {
                wind.draw(restartMessage);

                // hide the message after 2 seconds
                if (restartMessageClock.getElapsedTime().asSeconds() > 2)
                {
                    showRestartMessage = false;
                }
            }
        }
    }

    bool is_game_over() const
    {
        return gameover;

    }

    bool is_game_paused()
    {
        return is_paused;
    }

    Grid& get_grid()
    {
        return grid;
    }

    int shw_menu(RenderWindow& wind, int wind_w, int wind_h)
    {
        Font font;
        if (!font.loadFromFile("Fonts/joystix monospace.otf"))
        {
            cout << "unable to load font" << endl;
            exit(1);
        }

        Texture mt;
        if (!mt.loadFromFile("Textures/mb.jpg"))
        {
            cout << "unable to load menu background" << endl;
            exit(1);
        }

        Sprite menuBgSprite(mt);
        menuBgSprite.setScale(
            static_cast<float>(wind.getSize().x) / mt.getSize().x,
            static_cast<float>(wind.getSize().y) / mt.getSize().y
        );


        Text ttl; // main title
        ttl.setFont(font);
        ttl.setString("Snake Game");
        ttl.setCharacterSize(80);
        ttl.setFillColor(Color::Black);
        ttl.setPosition(wind_w * grid.get_cell_size() / 2 - ttl.getGlobalBounds().width / 2, 205);


        Text slgt, wrmt, pytht;//options slug, worm, python
        slgt.setFont(font);
        slgt.setString("Slug");
        slgt.setCharacterSize(30);
        slgt.setFillColor(Color::Black);
        slgt.setPosition(wind_w * grid.get_cell_size() / 4 - slgt.getGlobalBounds().width / 2, wind_h * grid.get_cell_size() / 2 + 20);

        wrmt.setFont(font);
        wrmt.setString("Worm");
        wrmt.setCharacterSize(30);
        wrmt.setFillColor(Color::Black);
        wrmt.setPosition(wind_w * grid.get_cell_size() / 2 - wrmt.getGlobalBounds().width / 2, wind_h * grid.get_cell_size() / 2 + 20);

        pytht.setFont(font);
        pytht.setString("Python");
        pytht.setCharacterSize(30);
        pytht.setFillColor(Color::Black);
        pytht.setPosition(3 * wind_w * grid.get_cell_size() / 4 - pytht.getGlobalBounds().width / 2, wind_h * grid.get_cell_size() / 2 + 20);


        Texture instructionsTexture, lbTexture;//for instructions
        if (!instructionsTexture.loadFromFile("Textures/i.png"))
        {
            cout << "Failed to load instructions icon!" << endl;
            exit(1);
        }
        if (!lbTexture.loadFromFile("Textures/i.png"))
        {
            cout << "Failed to load instructions icon!" << endl;
            exit(1);
        }

        Sprite instructionsSprite(instructionsTexture);
        instructionsSprite.setScale(0.05f, 0.05f);
        instructionsSprite.setPosition(wind_w * grid.get_cell_size() - instructionsSprite.getGlobalBounds().width - 80, wind_h * grid.get_cell_size() - instructionsSprite.getGlobalBounds().height - 30);

        while (wind.isOpen())// main menu loop

        {
            Event ev;
            while (wind.pollEvent(ev))
            {
                if (ev.type == Event::Closed)
                {
                    wind.close();
                    return -1;
                }
                if (ev.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(wind);
                    if (slgt.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        return 300;
                    }
                    if (wrmt.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        return 200;
                    }
                    if (pytht.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        return 100;
                    }
                    if (instructionsSprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        show_instructions(wind); // shows instructions screen
                    }
                }
            }

            wind.clear(Color::Black);
            wind.draw(menuBgSprite);
            wind.draw(ttl);
            wind.draw(slgt);
            wind.draw(wrmt);
            wind.draw(pytht);
            wind.draw(instructionsSprite);  //instructions 
            wind.display();
        }

        return 0;
    }






    void save_score()
    {

        ofstream file("C:/Users/amkia/Downloads/OOP_Project/Snake game/Snake game/highscores.txt", ios::app);

        if (!file.is_open())
        {
            cout << "unable to open file " << endl;
            return;
        }

        file << "score: " << getscore() << endl;
        file.close();
    }


    void endgame() override
    {
        gameOverSound.play();
        gameOverSound.setVolume(70);// game over sound
        backgroundMusic.stop();
        cout << "final Score: " << getscore() << endl;
        save_score();

    }



    void startgame() override
    {
        RenderWindow window(VideoMode(grid.get_grd_w() * grid.get_cell_size(), grid.get_grd_h() * grid.get_cell_size()), "Snake Game");

        int speed = shw_menu(window, grid.get_grd_w(), grid.get_grd_h());
        if (speed == -1) return;

        Clock clock;
        bool scr_svd = false;
        while (window.isOpen())
        {
            keyboard_input(window);

            if (!is_game_paused())
            {
                if (!is_game_over())
                {
                    if (clock.getElapsedTime().asMilliseconds() > speed)
                    {
                        clock.restart();
                        movement_snake();
                    }
                }
                else if (!scr_svd)
                {
                    endgame();
                    scr_svd = true;
                }
            }


            window.clear(Color::Black);
            render(window);
            window.display();
        }
    }

    void render(RenderWindow& wind) override
    {

        grid.draw_g(wind);


        draw_sprites(wind);
    }

    void show_instructions(RenderWindow& wind)
    {

        Texture mt;// menu background for instructions
        if (!mt.loadFromFile("Textures/mb.jpg")) {
            cout << "Unable to load menu background" << endl;
            exit(1);
        }

        Sprite menuBgSprite(mt);
        menuBgSprite.setScale(static_cast<float>(wind.getSize().x) / mt.getSize().x, static_cast<float>(wind.getSize().y) / mt.getSize().y
        );

        // instructions
        Font font;
        if (!font.loadFromFile("Fonts/joystix monospace.otf"))
        {
            cout << "Unable to load font" << endl;
            exit(1);
        }

        Text ins, i;
        ins.setFont(font);
        i.setFont(font);
        i.setString("Instructions");
        i.setFillColor(Color::Black);
        i.setPosition(280, 145);
        ins.setString("\n - Use arrow keys to move.\n - Eat food to grow longer. \n - Avoid hitting walls or yourself.\n - Press 'P' to pause / resume.\n - Click Restart to start a new game.");
        ins.setCharacterSize(25);
        ins.setFillColor(Color::Black);
        ins.setPosition(60, 200);


        Text bckto_menu;//back to menu button
        bckto_menu.setFont(font);
        bckto_menu.setString("Back to Menu");
        bckto_menu.setCharacterSize(30);
        bckto_menu.setFillColor(Color::Black);
        bckto_menu.setPosition(wind.getSize().x / 2 - bckto_menu.getGlobalBounds().width / 2, wind.getSize().y - 100);


        while (wind.isOpen()) // instructions screen
        {
            Event ev;
            while (wind.pollEvent(ev))
            {
                if (ev.type == Event::Closed)
                {
                    wind.close();
                }
                if (ev.type == Event::MouseButtonPressed)
                {
                    Vector2i mousePos = Mouse::getPosition(wind);
                    if (bckto_menu.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        return;  // Go back to the menu
                    }
                }
            }

            wind.clear(Color::White);
            wind.draw(menuBgSprite);  // instructions background
            wind.draw(ins);  //instructions text
            wind.draw(i);
            wind.draw(bckto_menu);  // back to menu button
            wind.display();
        }
    }


};