#pragma once

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include"position.h"

using namespace std;
using namespace sf;

class Leaderboard {
private:
    Player players[10];   // arr to store top 10 scores

    int num_plyrs;             // number of players  in the leaderboard
    Texture backgroundTexture;
    Sprite backgroundSprite;

public:
    Leaderboard() {
        num_plyrs = 0;

        if (!backgroundTexture.loadFromFile("Textures/back.jpg"))
        {
            cout << "Failed to load background image." << endl;
        }
        else
        {
            backgroundSprite.setTexture(backgroundTexture);
        }

        load_leaderboard();
    }

    void load_leaderboard()
    {
        fstream file("Player.txt", ios::in | ios::binary);
        if (file.is_open()) {
            num_plyrs = 0; // reset the player count
            while (file.read(reinterpret_cast<char*>(&players[num_plyrs]), sizeof(Player)) && num_plyrs < 10) {
                num_plyrs++;
            }
            file.close();
        }
        else {
            cout << "Player.txt not found. No players to display." << endl;
        }
    }

    void display_leaderboard(RenderWindow& window)
    {

        Position backgroundPosition(0, 0);//background
        backgroundSprite.setScale(
            float(window.getSize().x) / backgroundTexture.getSize().x,
            float(window.getSize().y) / backgroundTexture.getSize().y
        );
        backgroundSprite.setPosition(backgroundPosition.getx(), backgroundPosition.gety());
        window.draw(backgroundSprite);


        Position leaderboardSize(600.f, 400.f); //leaderboard box
        Position leaderboardPosition(
            (window.getSize().x - leaderboardSize.getx()) / 2,
            (window.getSize().y - leaderboardSize.gety()) / 2 + 50
        );

        RectangleShape leaderboardBox;
        leaderboardBox.setSize({ leaderboardSize.getx(), leaderboardSize.gety() });
        leaderboardBox.setFillColor(Color(255, 228, 196)); // Skin color
        leaderboardBox.setPosition(leaderboardPosition.getx(), leaderboardPosition.gety());
        window.draw(leaderboardBox);

        //font
        Font font;
        if (!font.loadFromFile("Fonts/joystix monospace.otf"))
        {
            cout << "Failed to load font." << endl;
            return;
        }

        // leaderboard title
        Text title("Leaderboard", font, 40);
        title.setFillColor(Color::Black);
        title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2, 50);
        window.draw(title);

        // Column headers
        Text nameHeader("Name", font, 24);
        nameHeader.setFillColor(Color::Black);
        nameHeader.setPosition(leaderboardPosition.getx() + 50, leaderboardPosition.gety() + 20);
        window.draw(nameHeader);

        Text scoreHeader("Score", font, 24);
        scoreHeader.setFillColor(Color::Black);
        scoreHeader.setPosition(leaderboardPosition.getx() + leaderboardSize.getx() - 150, leaderboardPosition.gety() + 20);
        window.draw(scoreHeader);

        // Display players
        for (int i = 0; i < num_plyrs; ++i) //name
        {

            Text playerName(players[i].getName(), font, 20);
            playerName.setFillColor(Color::Black);
            playerName.setPosition(leaderboardPosition.getx() + 50, leaderboardPosition.gety() + 60 + i * 30);
            window.draw(playerName);

            //score
            char scoreText[10];
            snprintf(scoreText, sizeof(scoreText), "%d", players[i].get_top_score());
            Text playerScore(scoreText, font, 20);
            playerScore.setFillColor(Color::Black);
            playerScore.setPosition(leaderboardPosition.getx() + leaderboardSize.getx() - 150, leaderboardPosition.gety() + 60 + i * 30);
            window.draw(playerScore);
        }
    }

    void update_leaderboard(const char* playerName, int score)
    {
        // add or update a player score
        bool playerExists = false;

        for (int i = 0; i < num_plyrs; ++i) {
            if (strcmp(players[i].getName(), playerName) == 0) {
                players[i].setTopScore(max(players[i].get_top_score(), score));
                playerExists = true;
                break;
            }
        }

        if (!playerExists && num_plyrs < 10)
        {
            players[num_plyrs] = Player(playerName, score);
            num_plyrs++;
        }

        // saving updated leaderboard back to the file
        saveLeaderboard();
    }

    void saveLeaderboard() {
        fstream file("Player.txt", ios::out | ios::binary);
        if (file.is_open()) {
            for (int i = 0; i < num_plyrs; ++i) {
                file.write(reinterpret_cast<char*>(&players[i]), sizeof(Player));
            }
            file.close();
        }
    }


    void startLeaderboard()
    {

        RenderWindow window(VideoMode(900, 700), "Leaderboard");
        window.setFramerateLimit(60);
        Player player;

        cin >> player;


        player.saveProgress(150); //example score for the player
        update_leaderboard(player.getName(), player.get_top_score()); // Update leaderboard with player score

        // main loop to display the leaderboard
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {

                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            // clear the window to display the leaderboard
            window.clear(Color(200, 200, 200));
            display_leaderboard(window);
            window.display();
        }
    }
};