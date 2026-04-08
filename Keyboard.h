#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SFML/Graphics.hpp>
#include <string>

class Keyboards {
private:
    static const int MAX_KEYS = 26;
    sf::RectangleShape keys[MAX_KEYS];
    sf::Text keyLabels[MAX_KEYS];
    sf::Font font;

public:
    Keyboards() {
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        // Define keyboard layout
        const char* keyLayout = "QWERTYUIOPASDFGHJKLZXCVBNM";

        // Initialize keys
        for (int i = 0; i < MAX_KEYS; ++i) {
            // Create key rectangle
            keys[i].setSize(sf::Vector2f(40, 40));
            keys[i].setFillColor(sf::Color(200, 200, 200));
            keys[i].setOutlineColor(sf::Color::Black);
            keys[i].setOutlineThickness(1);

            // Create key label
            keyLabels[i].setFont(font);
            keyLabels[i].setString(std::string(1, keyLayout[i]));
            keyLabels[i].setCharacterSize(20);
            keyLabels[i].setFillColor(sf::Color::Black);
        }

        // Position keys
        float startX = 100;
        float startY = 500;
        float keySpacing = 45;

        // First row (10 keys)
        for (int i = 0; i < 10; ++i) {
            float x = startX + i * keySpacing;
            keys[i].setPosition(x, startY);

            // Center text in key
            sf::FloatRect keyBounds = keys[i].getLocalBounds();
            sf::FloatRect textBounds = keyLabels[i].getLocalBounds();
            keyLabels[i].setPosition(
                x + (keyBounds.width - textBounds.width) / 2,
                startY + (keyBounds.height - textBounds.height) / 2 - textBounds.top
            );
        }

        // Second row (9 keys)
        for (int i = 10; i < 19; ++i) {
            float x = startX + 20 + (i - 10) * keySpacing;
            keys[i].setPosition(x, startY + keySpacing);

            // Center text in key
            sf::FloatRect keyBounds = keys[i].getLocalBounds();
            sf::FloatRect textBounds = keyLabels[i].getLocalBounds();
            keyLabels[i].setPosition(
                x + (keyBounds.width - textBounds.width) / 2,
                startY + keySpacing + (keyBounds.height - textBounds.height) / 2 - textBounds.top
            );
        }

        // Third row (7 keys)
        for (int i = 19; i < MAX_KEYS; ++i) {
            float x = startX + 40 + (i - 19) * keySpacing;
            keys[i].setPosition(x, startY + 2 * keySpacing);

            // Center text in key
            sf::FloatRect keyBounds = keys[i].getLocalBounds();
            sf::FloatRect textBounds = keyLabels[i].getLocalBounds();
            keyLabels[i].setPosition(
                x + (keyBounds.width - textBounds.width) / 2,
                startY + 2 * keySpacing + (keyBounds.height - textBounds.height) / 2 - textBounds.top
            );
        }
    }

    // Method to get key position
    sf::Vector2f getKeyPosition(int index) const {
        if (index >= 0 && index < MAX_KEYS) {
            return keys[index].getPosition();
        }
        return sf::Vector2f(0, 0);
    }

    // Method to set key position
    void setKeyPosition(int index, float x, float y) {
        if (index >= 0 && index < MAX_KEYS) {
            keys[index].setPosition(x, y);

            // Also adjust the label position to stay centered
            sf::FloatRect keyBounds = keys[index].getLocalBounds();
            sf::FloatRect textBounds = keyLabels[index].getLocalBounds();
            keyLabels[index].setPosition(
                x + (keyBounds.width - textBounds.width) / 2,
                y + (keyBounds.height - textBounds.height) / 2 - textBounds.top
            );
        }
    }

    // Method to change key color based on correctness
    void updateKeyColor(char letter, sf::Color color) {
        const char* keyLayout = "QWERTYUIOPASDFGHJKLZXCVBNM";
        for (int i = 0; i < MAX_KEYS; ++i) {
            if (std::tolower(keyLayout[i]) == std::tolower(letter)) {
                keys[i].setFillColor(color);
                break;
            }
        }
    }

    void draw(sf::RenderWindow& window) const {
        for (int i = 0; i < MAX_KEYS; ++i) {
            window.draw(keys[i]);
            window.draw(keyLabels[i]);
        }
    }
};

#endif