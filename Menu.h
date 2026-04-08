#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio/Music.hpp>
#include "Position.h"

using namespace sf;
using namespace std;

class Menu
{
private:
    static const int totalButtons = 3; // buttons in the menu
    Texture backgroundTexture;
    Sprite backgroundSprite;
    RectangleShape buttons[totalButtons]; //shape for buttons
    Font fnt;
    Text buttonTexts[totalButtons];
    Text questionText;
    bool buttonClicked[totalButtons]; //if a button has been clicked
   

    void createButton(const string& text, const Position& pos, const Position& size, int t)
    {
        buttons[t].setSize({ size.getx(), size.gety() });
        buttons[t].setPosition(pos.getx(), pos.gety());
        buttons[t].setFillColor(Color(181, 101, 29));

        buttonTexts[t].setFont(fnt);
        buttonTexts[t].setString(text);
        buttonTexts[t].setCharacterSize(32);
        buttonTexts[t].setFillColor(Color::Black);
        buttonTexts[t].setPosition(pos.getx() + 10, pos.gety() + 10);
    }

public:
    Music backgroundMusic; //background sound
    Menu() {
        
        for (int i = 0; i < totalButtons; ++i)
        {
            buttonClicked[i] = false;
        }

        // background image
        if (!backgroundTexture.loadFromFile("images/mnu.png"))
        {
            cout << "Failed to load background image" << endl;
        }

        // background sprite
        backgroundSprite.setTexture(backgroundTexture);
        FloatRect textureRect = backgroundSprite.getLocalBounds();
        backgroundSprite.setScale(900 / textureRect.width, 700 / textureRect.height);

        // font
        if (!fnt.loadFromFile("fonts/Pixeboy-z8XGD.ttf"))
        {
            cout << "Failed to load font" << endl;
        }

        // heading txt at 
        questionText.setFont(fnt);
        questionText.setString("Game Boy");
        questionText.setCharacterSize(100);
        questionText.setFillColor(Color::Black);
        FloatRect textBounds = questionText.getLocalBounds();
        questionText.setPosition((900 - textBounds.width) / 2, 250);

        // making of  buttons
        float bttn_w = 150;
        float bttn_h = 50;
        float spacing = 20;
        float totalWidth = (bttn_w * totalButtons) + (spacing * (totalButtons - 1));
        Position startPos((900 - totalWidth) / 2, 500);

        createButton("Snake", startPos, { bttn_w, bttn_h }, 0);
        createButton("Hangman", startPos + Position(bttn_w + spacing, 0), { bttn_w, bttn_h }, 1);
        createButton("Wordle", startPos + Position((bttn_w + spacing) * 2, 0), { bttn_w, bttn_h }, 2);

        // Load and play the background music
        if (!backgroundMusic.openFromFile("Sound Effects/menuback.ogg.opus"))
        {
            cout << "unable to load background music" << endl;;
        }
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    }

    void draw(RenderWindow& window)
    {
        //background
        window.draw(backgroundSprite);


        window.draw(questionText);//heading


        for (int i = 0; i < totalButtons; ++i) // buttons and their text
        {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
    }

    int handleInput(RenderWindow& wind)
    {
        Vector2i mousePos = Mouse::getPosition(wind);
        for (int i = 0; i < totalButtons; ++i) {
            if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                if (Mouse::isButtonPressed(Mouse::Left))
                {
                    if (!buttonClicked[i])
                    {
                        buttonClicked[i] = true; // mark button as clicked
                        return i + 1; //  index + 1 as identifier
                    }
                }
                else {

                    buttonClicked[i] = false; // resets clicked state
                }
            }
        }
        return 0; // no button was clicked
    }
};

#endif