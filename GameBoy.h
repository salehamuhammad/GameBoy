#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Player.h"
#include "Menu.h"
#include "Leaderboard.h"
#include "Snake Game.h" 
#include "Hangman Game.h"
#include "Wordle.h"

using namespace sf;

class GameBoy {
private:
    Player player;
    Menu menu;
    Leaderboard leaderboard;
    game* currentGame;
    RenderWindow window;

    // Method to set player's name directly without GUI
    void setPlayerName(const char* name) {
        player.set_nme(name);
    }

    // Method to start the selected game
    void startSelectedGame(int gameChoice) {
        window.close(); // Close any existing window
        window.create(VideoMode(900, 700), "Game");
        window.setFramerateLimit(60);

        // Create the appropriate game based on selection
        switch (gameChoice) {
        case 1: // Snake
            menu.backgroundMusic.stop();
            currentGame = new snake_game(45, 35, 20);
            break;
        case 2: // Hangman
            currentGame = new hangman_game();
            menu.backgroundMusic.stop();
            break;
        case 3: // Wordle
            currentGame = new Wordle();
            menu.backgroundMusic.stop();
            break;
        default:
            return;
        }

        // Start the game
        currentGame->startgame();

        // Game main loop
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            // Render game
            window.clear();
            currentGame->render(window);
            window.display();

            // Check if game is over
            if (currentGame) {
                int finalScore = currentGame->getscore();
                currentGame->endgame();

                delete currentGame;

                // Update leaderboard with player's score
                leaderboard.update_leaderboard(player.getName(), finalScore);

                break;
            }
        }

        // Display leaderboard after game
        leaderboard.startLeaderboard();
    }

public:
    GameBoy() : window(VideoMode(900, 700), "Game Center"), currentGame(nullptr) {
        // Set default player name
        setPlayerName("Player1");
    }

    void run() {
        // Create main game window
        window.create(VideoMode(900, 700), "Game Center");
        window.setFramerateLimit(60);

        // Main game loop
        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }

            // Clear the window
            window.clear();

            // Draw menu
            menu.draw(window);
            window.display();

            // Handle menu input
            int gameChoice = menu.handleInput(window);
            if (gameChoice > 0) {
                // Start the selected game
                startSelectedGame(gameChoice);
            }
        }
    }
};
