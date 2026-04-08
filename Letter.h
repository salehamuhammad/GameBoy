#ifndef LETTER_H
#define LETTER_H

#include <SFML/Graphics.hpp>
#include <string>

class Letter {
private:
    sf::RectangleShape box;
    sf::Text text;
    sf::Font font;
    char character;
    sf::Color boxColor;
    sf::Color borderColor;

public:
    Letter() : character(' '), boxColor(sf::Color(230, 230, 230)), borderColor(sf::Color(150, 150, 150)) {
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            throw std::runtime_error("Could not load font");
        }

        // Configure box with a more game-like appearance
        box.setSize(sf::Vector2f(60, 60));  // Slightly larger
        box.setFillColor(boxColor);
        box.setOutlineColor(borderColor);
        box.setOutlineThickness(3);  // Thicker border

        // Configure text
        text.setFont(font);
        text.setCharacterSize(35);  // Slightly larger text
        text.setFillColor(sf::Color::Black);
    }

    void setPosition(float x, float y) {
        box.setPosition(x, y);

        // Center text in box
        text.setPosition(
            x + (box.getSize().x - text.getLocalBounds().width) / 2,
            y + (box.getSize().y - text.getLocalBounds().height) / 2 - 5  // Slight vertical adjustment
        );
    }

    void setCharacter(char c) {
        character = toupper(c);  // Convert to uppercase
        text.setString(std::string(1, character));

        // Reposition text after setting character
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
        text.setPosition(
            box.getPosition().x + box.getSize().x / 2,
            box.getPosition().y + box.getSize().y / 2
        );
    }

    char getCharacter() const {
        return character;
    }

    void setColor(sf::Color color) {
        boxColor = color;
        box.setFillColor(boxColor);

        // Adjust border color for colored states
        if (color == sf::Color::Green) {
            borderColor = sf::Color(0, 128, 0);  // Darker green border
        }
        else if (color == sf::Color::Yellow) {
            borderColor = sf::Color(200, 200, 0);  // Darker yellow border
        }
        else if (color == sf::Color(100, 100, 100)) {  // Gray
            borderColor = sf::Color(70, 70, 70);  // Even darker gray border
        }
        box.setOutlineColor(borderColor);
    }

    sf::Color getColor() const {
        return boxColor;
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(box);
        if (character != ' ') {
            window.draw(text);
        }
    }
};

#endif