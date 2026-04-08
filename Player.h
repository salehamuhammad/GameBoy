#pragma once

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>


using namespace std;
using namespace sf;

class Player
{
private:
    char name[50];         // array for the name
    int hgh_scrs[10];    // arr to store top 10 scores
    bool name_set;          // chk if the player's name has been set

public:

    Player()
    {
        name[0] = '\0'; // make sure name is empty
        name_set = false;
        for (int i = 0; i < 10; ++i) {
            hgh_scrs[i] = 0;
        }
    }

    Player(const char* player_name, int score) {
        set_nme(player_name); // using setName to ensure proper copying
        name_set = true;
        for (int i = 0; i < 10; ++i) {
            hgh_scrs[i] = 0;
        }
        if (score > 0) {
            hgh_scrs[0] = score; // setting initial score
        }
    }


    void set_nme(const char* player_name)
    {
        if (player_name != nullptr)
        {
            size_t length = 0;
            while (player_name[length] != '\0' && length < sizeof(name) - 1)
            {
                name[length] = player_name[length];
                ++length;
            }
            name[length] = '\0'; //null-termination
            name_set = true;
        }
    }

    void saveProgress(int score) {
        if (name_set) {
            // Save the player's name and score to the file
            ofstream file("Player.txt", ios::out | ios::app);
            if (file.is_open()) {
                file << name << '\n'; // Write the player's name
                file << score << '\n'; // Write the score on the next line
                file.close();
                cout << "Progress saved successfully!" << endl;
            }
            else {
                cout << "Failed to save progress." << endl;
            }
        }
        else {
            cout << "Name not set. Cannot save progress." << endl;
        }
    }


    const char* getName() const
    {
        return name;
    }

    int get_top_score() const
    {
        return hgh_scrs[0]; // top score is stored in the first position
    }
    friend istream& operator>>(istream& input, Player& p)
    {
        cout << "Enter name: " << endl;
        char temp_name[50];
        input.getline(temp_name, sizeof(temp_name));
        p.set_nme(temp_name);
        return input;
    }
    void setTopScore(int score)
    {
        // Chck if the new score should be added to the high scores
        for (int i = 0; i < 10; ++i)
        {
            if (score > hgh_scrs[i])
            {

                for (int j = 9; j > i; --j)  // shifting lower scores down to make space for the new score
                {
                    hgh_scrs[j] = hgh_scrs[j - 1];
                }
                hgh_scrs[i] = score; // new score at the correct position
                break;
            }
        }
    }
};