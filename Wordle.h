#ifndef WORDLE_H
#define WORDLE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Letter.h"
#include "WordDictionary.h"
#include "game.h"
#include "Position.h"
#include "Keyboard.h" // New include for virtual keyboard

using namespace std;
using namespace sf;

class Wordle : public game 
{
private:
    char targetWord[6];
    static const int maxRows = 6;
    static const int maxCols = 5;
    Letter grid[maxRows][maxCols];
    int currentRow;
    int currentCol;
    RectangleShape background;
    Font font;
    Text titleText;
    Text attemptsText;
    Text invalidWordText;
    RectangleShape gameBoard;
    Position backgroundSize;
    Position gameBoardSize;
    Position gameBoardPosition;
    bool gameWon;
    bool gameLost;
    WordDictionary dictionary;
    sf::Music backgroundMusic;

    Texture pauseTexture;
    Sprite pauseSprite;
    bool is_paused;
    Texture instructionsTexture;
    Sprite instructionsSprite;

    Keyboards virtualKeyboard; // Virtual keyboard member

public:
    Wordle() :
        currentRow(0),
        currentCol(0),
        backgroundSize(900, 700),
        gameBoardSize(350, 400),
        gameBoardPosition(225, 80),
        gameWon(false),
        gameLost(false),
        is_paused(false)
    {
        srand(static_cast<unsigned>(time(nullptr)));
        selectRandomWord();

        // Font loading
        if (!font.loadFromFile("Fonts/Melted Monster.ttf"))
        {
            cout << "Could not load font";
        }

        // Pause button texture
        if (!pauseTexture.loadFromFile("Textures/pause.png")) {
            cout << "Could not load pause texture";
        }
        pauseSprite.setTexture(pauseTexture);
        pauseSprite.setScale(0.2f, 0.2f);
        pauseSprite.setPosition(800, 10);

        // Instructions button texture
        if (!instructionsTexture.loadFromFile("Textures/i.png")) {
            cout << "Could not load instructions texture";
        }
        instructionsSprite.setTexture(instructionsTexture);
        instructionsSprite.setScale(0.05f, 0.05f);
        instructionsSprite.setPosition(10, 10);

        // Background setup
        setBackground("images/bg.jpg");

        background.setSize(sf::Vector2f(backgroundSize.getx(), backgroundSize.gety()));
        background.setFillColor(sf::Color(240, 240, 240));

        // Game board setup
        gameBoard.setSize(sf::Vector2f(gameBoardSize.getx(), gameBoardSize.gety()));
        gameBoard.setFillColor(sf::Color(255, 255, 255));
        gameBoard.setOutlineColor(sf::Color(200, 200, 200));
        gameBoard.setOutlineThickness(5);
        gameBoard.setPosition(gameBoardPosition.getx(), gameBoardPosition.gety());

        // Title text
        titleText.setFont(font);
        titleText.setString("WORDLE");
        titleText.setCharacterSize(50);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(330, 20);

        // Attempts text
        attemptsText.setFont(font);
        attemptsText.setString("Attempts Left: 6");
        attemptsText.setCharacterSize(20);
        attemptsText.setFillColor(sf::Color::White);
        attemptsText.setPosition(60, 40);

        // Invalid word text
        invalidWordText.setFont(font);
        invalidWordText.setString("Invalid word! Try again.");
        invalidWordText.setCharacterSize(20);
        invalidWordText.setFillColor(sf::Color::Red);
        invalidWordText.setPosition(250, 550);

        // Background music
        if (!backgroundMusic.openFromFile("Sound Effects/bw.ogg.opus")) {
            cout << "Could not load background music";
        }
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    }

    void show_instructions(RenderWindow& wind) {
        RenderWindow instructionsWindow(VideoMode(600, 500), "Wordle Instructions");

        Text instructionText;
        instructionText.setFont(font);
        instructionText.setCharacterSize(20);
        instructionText.setFillColor(sf::Color::Black);
        instructionText.setPosition(50, 50);
        instructionText.setString(
            "Wordle Game Instructions:\n\n"
            "1. Your goal is to guess a 5-letter word in 6 attempts.\n\n"
            "2. After each guess:\n"
            "   - GREEN means the letter is correct and in the right position\n"
            "   - YELLOW means the letter is in the word but wrong position\n"
            "   - GRAY means the letter is not in the word\n\n"
            "3. Use keyboard to enter letters:\n"
            "   - Type letters to fill the current row\n"
            "   - Press ENTER to submit your guess\n"
            "   - Use BACKSPACE to delete letters\n\n"
            "4. You win by guessing the word correctly\n"
            "5. You lose if you can't guess the word in 6 attempts\n\n"
            "Good Luck!"
        );

        while (instructionsWindow.isOpen()) {
            Event event;
            while (instructionsWindow.pollEvent(event)) {
                if (event.type == Event::Closed)
                    instructionsWindow.close();
            }

            instructionsWindow.clear(sf::Color::White);
            instructionsWindow.draw(instructionText);
            instructionsWindow.display();
        }
    }

    void draw_pause_button(RenderWindow& window) {
        window.draw(pauseSprite);
    }

    void handleInput(sf::Event& event, sf::RenderWindow& window, bool& gameOver) {
        // If game is already won or lost, don't accept input
        if (gameWon || gameLost) {
            gameOver = true;
            return;
        }

        // Check for mouse click
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                // Check for pause button
                if (pauseSprite.getGlobalBounds().contains(mousePos)) {
                    is_paused = !is_paused;
                    return;
                }

                // Check for instructions button
                if (instructionsSprite.getGlobalBounds().contains(mousePos)) {
                    show_instructions(window);
                    return;
                }

                // Check for keyboard input
                if (isMouseOverKeyboard(mousePos)) {
                    char input = getKeyboardInput(mousePos);
                    if (input != '\0') {
                        handleCharInput(input, gameOver);
                    }
                }
            }
        }

        // Keyboard input handling
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Enter) {
                handleCharInput('\n', gameOver);
            }
            else if (event.key.code == sf::Keyboard::BackSpace) {
                handleCharInput('\b', gameOver);
            }
        }
        else if (event.type == sf::Event::TextEntered) {
            char input = static_cast<char>(event.text.unicode);
            if (input >= 'a' && input <= 'z') {
                handleCharInput(input, gameOver);
            }
        }
    }

    void handleCharInput(char input, bool& gameOver) {
        // If game is already won or lost, don't accept input
        if (gameWon || gameLost) {
            gameOver = true;
            return;
        }

        if (input == '\n') {
            // Only process if we have a complete word
            if (currentCol == maxCols) {
                // Extract entered word
                char enteredWord[6];
                for (int i = 0; i < maxCols; ++i) {
                    enteredWord[i] = grid[currentRow][i].getCharacter();
                }
                enteredWord[maxCols] = '\0';

                // Check if the word is valid
                if (dictionary.isValidWord(enteredWord)) {
                    updateLetterColors();

                    // Check if won
                    if (gameWon) {
                        gameOver = true;
                        return;
                    }

                    // Move to next row
                    currentRow++;
                    currentCol = 0;

                    // Update attempts left
                    char attemptsMessage[20];
                    int remainingAttempts = maxRows - currentRow;
                    snprintf(attemptsMessage, sizeof(attemptsMessage), "Attempts Left: %d", remainingAttempts);
                    attemptsText.setString(attemptsMessage);

                    // Check if out of attempts
                    if (currentRow >= maxRows) {
                        gameLost = true;
                        gameOver = true;
                    }
                }
                else {
                    // Show invalid word message
                    invalidWordText.setString("Invalid word! Try again.");
                }
            }
        }
        else if (input == '\b') {
            // Backspace
            if (currentCol > 0) {
                currentCol--;
                grid[currentRow][currentCol].setCharacter(' ');
            }
        }
        else if (currentCol < maxCols) {
            // Add letter
            grid[currentRow][currentCol].setCharacter(input);
            currentCol++;
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(background);
        window.draw(gameBoard);
        window.draw(titleText);
        window.draw(attemptsText);

        // Const cast is necessary to call non-const methods
        Wordle* nonConstThis = const_cast<Wordle*>(this);
        nonConstThis->virtualKeyboard.draw(window);

        for (int i = 0; i < maxRows; ++i) {
            for (int j = 0; j < maxCols; ++j) {
                grid[i][j].draw(window);
            }
        }

        nonConstThis->draw_pause_button(window);
        window.draw(instructionsSprite);

        // Game over texts
        if (gameWon) {
            Text winText;
            winText.setFont(font);
            winText.setString("Congratulations! You won!");
            winText.setCharacterSize(30);
            winText.setFillColor(sf::Color(0, 128, 0));
            winText.setPosition(250, 550);
            window.draw(winText);
        }
        else if (gameLost) {
            Text loseText;
            loseText.setFont(font);
            loseText.setString("Game Over! The word was: " + std::string(targetWord));
            loseText.setCharacterSize(30);
            loseText.setFillColor(sf::Color::Red);
            loseText.setPosition(200, 550);
            window.draw(loseText);
        }

        // Draw pause text if game is paused
        if (is_paused) {
            Text pausedText;
            pausedText.setFont(font);
            pausedText.setString("GAME PAUSED");
            pausedText.setCharacterSize(50);
            pausedText.setFillColor(sf::Color::Red);
            pausedText.setPosition(250, 300);
            window.draw(pausedText);
        }
    }

    void startgame() override {
        RenderWindow window(VideoMode(900, 700), "Wordle");
        Event event;
        bool gameOver = false;

        initialize();

        while (window.isOpen()) {
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                handleInput(event, window, gameOver);
            }

            window.clear(sf::Color::White);
            draw(window);
            window.display();

            if (gameOver) {
                endgame();
                window.close();
            }
        }
    }

    void endgame() override {
        backgroundMusic.stop();
        if (gameWon) {
            cout << "Congratulations! You won!" << std::endl;
        }
        else if (gameLost) {
            cout << "Game Over! The word was: " << targetWord << std::endl;
        }
    }

    void render(RenderWindow& win) override {
        draw(win);
    }

    void selectRandomWord() {
        const char* randomWord = dictionary.getRandomWord();
        for (int i = 0; i < maxCols; ++i) {
            targetWord[i] = randomWord[i];
        }
        targetWord[maxCols] = '\0';
    }

    void initialize() {
        for (int i = 0; i < maxRows; ++i) {
            for (int j = 0; j < maxCols; ++j) {
                Position cellPosition = Position(250 + j * 65, 100 + i * 65);
                grid[i][j].setPosition(cellPosition.getx(), cellPosition.gety());
                grid[i][j].setCharacter(' ');
                grid[i][j].setColor(sf::Color::White);
            }
        }
        currentRow = 0;
        currentCol = 0;
        gameWon = false;
        gameLost = false;
        selectRandomWord();
        attemptsText.setString("Attempts Left: 6");
    }

    void setBackground(const std::string& imagePath) {
        sf::Texture* backgroundTexture = new sf::Texture();
        if (!backgroundTexture->loadFromFile(imagePath)) {
            throw runtime_error("Failed to load background image: " + imagePath);
        }
        background.setTexture(backgroundTexture);
    }

    bool isGameWon() const { return gameWon; }
    bool isGameLost() const { return gameLost; }

    void updateLetterColors() {
        char tempTargetWord[6];
        char enteredWord[6];
        for (int i = 0; i < maxCols; ++i) {
            tempTargetWord[i] = targetWord[i];
            enteredWord[i] = grid[currentRow][i].getCharacter();
        }
        tempTargetWord[maxCols] = '\0';
        enteredWord[maxCols] = '\0';

        bool letterUsed[5] = { false };

        // First pass: mark correct letters (green)
        for (int i = 0; i < maxCols; ++i) {
            if (enteredWord[i] == tempTargetWord[i]) {
                grid[currentRow][i].setColor(sf::Color::Green);
                letterUsed[i] = true;
            }
        }

        // Second pass: mark present but misplaced letters (yellow)
        for (int i = 0; i < maxCols; ++i) {
            if (grid[currentRow][i].getColor() == sf::Color::Green) continue;

            for (int j = 0; j < maxCols; ++j) {
                if (enteredWord[i] == tempTargetWord[j] && !letterUsed[j]) {
                    grid[currentRow][i].setColor(sf::Color::Yellow);
                    letterUsed[j] = true;
                    break;
                }
            }

            // If letter not found, mark as gray
            if (grid[currentRow][i].getColor() == sf::Color::White) {
                grid[currentRow][i].setColor(sf::Color(100, 100, 100));
            }
        }

        // Update virtual keyboard key colors
        for (int i = 0; i < maxCols; ++i) {
            sf::Color keyColor = grid[currentRow][i].getColor();
            virtualKeyboard.updateKeyColor(enteredWord[i], keyColor);
        }

        // Check if the word is completely correct
        gameWon = true;
        for (int i = 0; i < maxCols; ++i) {
            if (enteredWord[i] != targetWord[i]) {
                gameWon = false;
                break;
            }
        }
    }

    bool isMouseOverKeyboard(const sf::Vector2f& mousePos) const {
        // Rough bounds checking for the virtual keyboard
        return (mousePos.y >= 500 && mousePos.y <= 650 &&
            mousePos.x >= 100 && mousePos.x <= 600);
    }

    char getKeyboardInput(const sf::Vector2f& mousePos) {
        const char* keyLayout = "QWERTYUIOPASDFGHJKLZXCVBNM";

        for (int i = 0; i < 26; ++i) {
            sf::Vector2f keyPos = virtualKeyboard.getKeyPosition(i);
            sf::FloatRect keyBounds(keyPos.x, keyPos.y, 40, 40);

            if (keyBounds.contains(mousePos)) {
                // Convert to lowercase for input
                return std::tolower(keyLayout[i]);
            }
        }

        return '\0';
    }
};

#endif // WORDLE_H