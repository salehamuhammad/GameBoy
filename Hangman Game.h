#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include"Hangman Figure.h"
#include"Game.h"
#include"Position.h"
using namespace std;
using namespace sf;



class hangman_game : public game
{
private:
    hangman_figure hmf;
    char** dictionary;
    const char* word;
    char* guessed_word;
    int lives;
    char guessed_letters[26];
    int guessed_wrd_count;
    bool timed;
    int hints;

    const char* saved_word;
    Texture menuBackgroundTexture;
    Sprite menuBackgroundSprite;

    Texture BackgroundTexture;
    Sprite  BackgroundSprite;
    Texture pausTexture;
    Sprite pausSprite;
    Texture hntTexture;
    Sprite hntSprite;
    Time paused_time;
    bool is_paused;
    Time total_elapsed_time; // var to hold total elapsed time
    Clock game_clock;
    const int ttl_words = 100;
    Font f1;
    Text scr_t;
    Text scrval_t;

    Music backgroundMusic;
    SoundBuffer winBuffer;
    SoundBuffer loseBuffer;
    Sound winSound;
    Sound loseSound;



public:

    hangman_game() : lives(6), guessed_wrd_count(0), timed(false), hints(3)
    {
        srand(static_cast<unsigned>(time(0)));

        dictionary = new char* [ttl_words];
        for (int i = 0; i < ttl_words; i++)
        {
            dictionary[i] = new char[30];
        }

        const char* words[] = {
           "programming", "is", "fun", "fast", "nuces", "computer", "science", "students", "are", "tired", "genius",
        "university", "semester", "applied", "physics", "calculus", "fundamentals", "algorithms", "lab", "lectures",
        "functional", "stem", "fried", "debugging", "java", "python", "cplusplus", "html", "css", "javascript",
        "web", "development", "software", "information", "technology", "graphics", "analysis", "cloud", "virtualization",
        "robotics", "internet", "things", "big", "integration", "data", "variables", "functions", "education", "degree", "research",
        "professor", "dropout", "deprived", "peekaboo", "sour", "candy", "watermelon", "twinkle", "star", "shine", "perfect",
        "night", "umbrella", "xylophone", "quasar", "jazz", "flamingo", "blizzard", "vortex", "galaxy", "sphinx", "cucumber", "whiskey",
        "jigsaw", "hazard", "bungalow", "kaleidoscope", "quicksand", "razzmatazz", "puzzle", "bazaar", "jungle", "syndrome",
        "waltz", "fizzle", "blitz", "haphazard", "juxtapose", "buzzard", "squeamish", "razor", "gizmo", "vivid", "muffin", "quilt",
        "zeppelin", "gazelle", "whimsical", "jubilee", "food"
        };

        for (int i = 0; i < ttl_words; i++)
        {
            const char* src = words[i];
            char* dest = dictionary[i];
            int j = 0;
            while (src[j] != '\0') {
                dest[j] = src[j];
                j++;
            }
            dest[j] = '\0';
        }

        word = nullptr;
        guessed_word = nullptr;


        if (!menuBackgroundTexture.loadFromFile("Textures/menu.png"))//load menu
        {
            cout << "unable to load menu background texture" << endl;
            exit(-1);
        }
        menuBackgroundSprite.setTexture(menuBackgroundTexture);
        menuBackgroundSprite.setColor(Color(250, 250, 250, 50));

        FloatRect BackgroundBounds = menuBackgroundSprite.getLocalBounds();
        BackgroundSprite.setScale(800.0f / BackgroundBounds.width, 700.0f / BackgroundBounds.height);

        if (!BackgroundTexture.loadFromFile("Textures/background.jpg"))//load background
        {
            cout << "Failed to load menu background texture!" << endl;
            exit(-1);
        }
        BackgroundSprite.setTexture(BackgroundTexture);
        BackgroundSprite.setColor(Color(250, 250, 250, 50));
        FloatRect backgroundBounds = BackgroundSprite.getLocalBounds();
        BackgroundSprite.setScale(800.0f / backgroundBounds.width, 700.0f / backgroundBounds.height);

        if (!pausTexture.loadFromFile("Textures/pause.png"))//load pause button
        {
            cout << "unable to load pause button texture" << endl;
            exit(-1);
        }
        pausSprite.setTexture(pausTexture);
        pausSprite.setScale(0.2f, 0.2f);  // button size

        if (!hntTexture.loadFromFile("Textures/hint.png"))
        {
            cout << "unable to load hint button texture" << endl;
            exit(-1);
        }
        hntSprite.setTexture(hntTexture);
        hntSprite.setScale(0.3f, 0.3f);


        if (!f1.loadFromFile("Fonts/Melted Monster.ttf"))
        {
            cout << "unable to load font" << endl;
            exit(1);
        }

        scr_t.setFont(f1);
        scr_t.setCharacterSize(30);
        scr_t.setFillColor(Color::Black);
        scr_t.setString("Score");
        scr_t.setPosition((45 * 20) - 150, 50);

        scrval_t.setFont(f1);
        scrval_t.setCharacterSize(40);
        scrval_t.setFillColor(Color::Black);
        scrval_t.setPosition(scr_t.getPosition().x + 50, scr_t.getPosition().y + 40);

        update_score_text();

        // music
        if (!backgroundMusic.openFromFile("Sound Effects/background.wav")) {
            cout << "Error loading background music" << endl;
        }


        if (!winBuffer.loadFromFile("Sound Effects/win.wav"))
        {
            cout << "Error loading win sound" << endl;
        }
        winSound.setBuffer(winBuffer);






        if (!loseBuffer.loadFromFile("Sound Effects/lose.wav")) {
            cout << "Error loading lose sound" << endl;
        }
        loseSound.setBuffer(loseBuffer);

        backgroundMusic.setLoop(true); //background music
        backgroundMusic.play();        // start playing


        // Set volume levels
        backgroundMusic.setVolume(50);
        winSound.setVolume(70);
        loseSound.setVolume(70);
        //loseLifeSound.setVolume(70);
    }
    void save_score()
    {
        ofstream file("C:/Users/amkia/Downloads/OOP_Project/hangman/Hangman game/highscores.txt", ios::app);

        // Check if the file is open and handle errors
        if (!file) {
            cerr << "Unable to open file for writing at the specified path: "
                << "C:/Users/amkia/Downloads/OOP_Project/hangman/Hangman game/highscores.txt" << endl;
            return;
        }

        file << "score: " << getscore() << endl;
        file.close();
        cout << "Score saved successfully." << endl;
    }

    void update_score_text()
    {
        scrval_t.setString(to_string(getscore()));
    }

    void show_instructions(RenderWindow& wind)
    {
        Texture mt; // menu background for instructions
        if (!mt.loadFromFile("Textures/background.jpg"))
        {
            cout << "Unable to load menu background" << endl;
            exit(1);
        }

        Sprite menuBgSprite(mt);
        menuBgSprite.setScale(
            static_cast<float>(wind.getSize().x) / mt.getSize().x,
            static_cast<float>(wind.getSize().y) / mt.getSize().y
        );



        Font font;// Instructions font
        if (!font.loadFromFile("Fonts/Melted Monster.ttf")) {
            cout << "Unable to load font" << endl;
            exit(1);
        }

        // title
        Text i;
        i.setFont(font);
        i.setString("Instructions");
        i.setCharacterSize(40);
        i.setFillColor(Color::Black);
        i.setPosition(wind.getSize().x / 2 - i.getGlobalBounds().width / 2, 80);


        RectangleShape instrBox; // white background for instructions
        instrBox.setSize({ static_cast <float>(720.f), static_cast <float>(420.f) });
        instrBox.setFillColor(Color::White);
        instrBox.setOutlineColor(Color::Black);
        instrBox.setOutlineThickness(2);
        instrBox.setPosition(
            (wind.getSize().x - instrBox.getSize().x) / 2,
            (wind.getSize().y - instrBox.getSize().y) / 2
        );


        Text ins; // Instructions text
        ins.setFont(font);
        ins.setString(
            "\n"
            "1. Gameplay:\n"
            "   - You will see blanks representing the letters in the hidden word.\n"
            "   - Click or type a letter to make a guess.\n"
            "   - If the guessed letter is correct, it will fill in the blanks.\n"
            "   - If the guessed letter is incorrect, a part of the hangman is drawn.\n"
            "2. Winning:\n"
            "   - You win if you guess the word completely before a hangman is drawn.\n"
            "3. Losing:\n"
            "   - You lose if the hangman drawing is completed .\n"
            "4. Hints:\n"
            "   - Use a hint button,to help you guess the word.\n"
            "5. Timed Mode:\n"
            "   - In timed mode, guess the word within a time limit.\n"
            "6. Untimed Mode:\n"
            "   - In untimed mode, there is no time limit.\n\n"

        );
        ins.setCharacterSize(20);
        ins.setFillColor(Color::Black);
        ins.setPosition(
            instrBox.getPosition().x + 20, instrBox.getPosition().y);


        Text bckto_menu; // back to menu button
        bckto_menu.setFont(font);
        bckto_menu.setString("Back to Menu");
        bckto_menu.setCharacterSize(30);
        bckto_menu.setFillColor(Color::Black);
        bckto_menu.setPosition(
            wind.getSize().x / 2 - bckto_menu.getGlobalBounds().width / 2,
            instrBox.getPosition().y + instrBox.getSize().y + 20
        );


        while (wind.isOpen()) // Instructions screen loop
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
                        return; //back to the menu
                    }
                }
            }

            wind.clear(Color::White);
            wind.draw(menuBgSprite);  //background
            wind.draw(instrBox);    // instruction box
            wind.draw(ins);               // instructions text
            wind.draw(i);                 //  title
            wind.draw(bckto_menu);  //  back to menu button
            wind.display();
        }
    }


    const char* generate_random_word()
    {

        int index = rand() % ttl_words;
        word = dictionary[index]; // word at the random index
        return word;
    }

    ~hangman_game()
    {
        for (int i = 0; i < 100; i++)
        {
            delete[] dictionary[i];
        }
        delete[] dictionary;
        if (guessed_word) delete[] guessed_word;
    }

    void set_timed(bool is_timed)
    {
        timed = is_timed;
        if (timed)
        {
            game_clock.restart();
        }
    }
    void display_game_state(RenderWindow& wind, Font& ft)
    {
        Text guessed_text("", ft, 30);
        guessed_text.setFillColor(Color::Black);

        char display_text[100]; // creating guessed word with spaces
        int i = 0, j = 0;
        while (guessed_word[i] != '\0') {
            display_text[j++] = guessed_word[i++];
            display_text[j++] = ' ';
        }
        display_text[j] = '\0';

        guessed_text.setString(display_text);

        FloatRect textBounds = guessed_text.getLocalBounds();
        float x_position = (wind.getSize().x - textBounds.width) / 2;
        float y_position = wind.getSize().y - 150;

        guessed_text.setPosition(x_position, y_position);
        wind.draw(guessed_text);

        Text lives_text("Lives: " + to_string(lives), ft, 30); // for showing lives
        lives_text.setFillColor(Color::Black);
        lives_text.setPosition(600, 50);
        wind.draw(lives_text);

        Text htxt("Hints: " + to_string(hints), ft, 30); // for showing hints
        htxt.setFillColor(Color::Black);
        htxt.setPosition(50, 150);
        wind.draw(htxt);

        if (timed) {
            int remaining_time = 120 - static_cast<int>(get_total_elapsed_time().asSeconds()); //using total elapsed time
            if (remaining_time <= 0)
            {
                remaining_time = 0;
                lives = 0; // end the game if time runs out
            }

            Text timer_text("Time: " + to_string(remaining_time) + "s", ft, 30);
            timer_text.setFillColor(Color::Black);
            timer_text.setPosition(50, 50);
            wind.draw(timer_text);
        }

        char guessed_letter[100] = "Guessed: ";
        int idx = 9;

        for (int t = 0; t < guessed_wrd_count; ++t)
        {
            guessed_letter[idx++] = guessed_letters[t];
            guessed_letter[idx++] = ' ';
        }
        guessed_letter[idx] = '\0';//termnating the string

        Text gt(guessed_letter, ft, 30);
        gt.setFillColor(Color::Black);
        gt.setPosition(50, 100);
        wind.draw(gt);
        draw_score(wind);//score
        draw_pause_button(wind);//button
    }

    void initialize_guessed_word()
    {
        int length = 0;
        while (word[length] != '\0') length++;
        guessed_word = new char[length + 1];
        for (int i = 0; i < length; i++) {
            guessed_word[i] = '_';
        }
        guessed_word[length] = '\0';
    }

    bool is_game_won()
    {
        for (int i = 0; guessed_word[i] != '\0'; i++)
        {
            if (guessed_word[i] == '_')
                return false;
        }
        return true;
    }

    bool is_game_lost()
    {
        return lives == 0;
    }

    void draw_hangman(RenderWindow& window)
    {
        hmf.getsprite().setTexture(*hmf.gettexture(6 - lives));


        float spr_x = (window.getSize().x - hmf.getsprite().getGlobalBounds().width) / 2;
        float spr_y = (window.getSize().y - hmf.getsprite().getGlobalBounds().height) / 3 + 50;

        hmf.getsprite().setPosition(spr_x, spr_y);
        window.draw(hmf.getsprite());
    }


    void reset_game()
    {
        lives = 6;
        guessed_wrd_count = 0;
        for (int i = 0; i < 26; ++i) guessed_letters[i] = '\0';
        if (guessed_word) delete[] guessed_word;
        word = generate_random_word();
        saved_word = word;
        initialize_guessed_word();
        if (timed) game_clock.restart();
    }

    const char* get_saved_word() const
    {
        return saved_word;
    }

    void draw_menu_background(RenderWindow& win)
    {

        Vector2u windowSize = win.getSize();

        Vector2u textureSize = menuBackgroundTexture.getSize();

        float sx = static_cast<float>(windowSize.x) / textureSize.x;
        float sy = static_cast<float>(windowSize.y) / textureSize.y;

        menuBackgroundSprite.setScale(sx, sy);

        win.draw(menuBackgroundSprite);
    }


    void draw_background(RenderWindow& wind)
    {
        Vector2u windowSize = wind.getSize();
        Vector2u textureSize = BackgroundTexture.getSize();
        float sx = static_cast<float>(windowSize.x) / textureSize.x;
        float sy = static_cast<float>(windowSize.y) / textureSize.y;

        BackgroundSprite.setScale(sx, sy); // set background 

        wind.draw(BackgroundSprite);
    }

    bool provide_hint()
    {
        if (hints <= 0) // no hints left
            return false;

        const int max_length = 100;
        int unguessed_indices[max_length];
        int unguessed_count = 0;


        for (int i = 0; word[i] != '\0'; ++i) // indices of unguessed letters
        {
            if (guessed_word[i] == ' ') // if the letter is still a ''
            {
                unguessed_indices[unguessed_count++] = i; // add the index to the list
            }
        }

        if (unguessed_count == 0) //if no letters left to guess
        {
            return false;
        }


        srand(time(0)); // seed the random number generator
        int random_index = unguessed_indices[rand() % unguessed_count];


        guessed_word[random_index] = word[random_index];//random letter at the selected index

        hints--;
        lives--;

        return true;
    }
    int get_remaining_hints() const
    {
        return hints;
    }

    Sprite& getPauseButtonSprite()
    {
        return pausSprite;
    }

    Texture& getPauseButtonTexture()
    {
        return pausTexture;
    }

    void draw_pause_button(RenderWindow& wind)
    {
        //pause button at the top-right corner with a margin
        float margin = 10.0f;
        float x_position = wind.getSize().x - pausSprite.getGlobalBounds().width - margin;
        float y_position = margin;
        pausSprite.setPosition(x_position, y_position);

        wind.draw(pausSprite);
    }

    void draw_hint_button(RenderWindow& wind)
    {
        //hint button on the bottom-right corner with a margin
        float margin = 10.0f;
        float x_position = wind.getSize().x - hntSprite.getGlobalBounds().width - margin;
        float y_position = wind.getSize().y - hntSprite.getGlobalBounds().height - margin;
        hntSprite.setPosition(x_position, y_position);

        wind.draw(hntSprite);
    }

    void draw_score(RenderWindow& wind)
    {
        wind.draw(scr_t);// score
        wind.draw(scrval_t);// score value
    }
    Sprite& getHintButtonSprite()
    {
        return hntSprite;
    }


    void handle_input(Event& e, RenderWindow& window, bool& in_menu, bool& in_game, bool& in_end_screen,
        bool& game_won, Font& font, Text& opt1, Text& opt2, bool& is_paused, Sprite& instructionsSprite)//for keyboard and mouse inputs
    {
        if (e.type == Event::Closed)
        {
            window.close();
        }


        if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
        {
            Vector2i mousePos = Mouse::getPosition(window);


            if (pausSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) // Check for pause button
            {
                if (is_paused)
                {
                    resume_timer();
                    is_paused = false;
                }
                else
                {
                    pause_timer();
                    is_paused = true;
                }
            }

            // menu option click
            if (in_menu)
            {
                if (opt1.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                {
                    set_timed(true);
                    reset_game();
                    in_menu = false;
                    in_game = true;
                }

                else if (opt2.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))
                {
                    set_timed(false);
                    reset_game();
                    in_menu = false;
                    in_game = true;
                }
            }

            // instructions button click
            if (instructionsSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))// Function to show instructions
            {
                show_instructions(window);
            }


            if (hntSprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)))//hint button click
            {
                if (provide_hint())
                {
                    cout << "Hint provided!" << endl;
                }
                else {
                    cout << "No hints left!" << endl;
                }
            }
        }


        if (e.type == Event::KeyPressed)// keyboard input for guesses
        {
            if (in_game)
            {
                if (e.key.code >= Keyboard::A && e.key.code <= Keyboard::Z)
                {
                    char guessed_letter = static_cast<char>(e.key.code - Keyboard::A + 'a');
                    if (!is_paused)
                    {
                        process_guess(guessed_letter);
                    }
                }
            }
        }
    }

    void startgame() override
    {
        RenderWindow window(VideoMode(900, 700), "Hangman Game");
        Event e;

        bool in_menu = true, in_game = false, in_end_screen = false;
        bool game_won = false, is_paused = false;

        Font font, font1;
        if (!font.loadFromFile("Fonts/arial.ttf") || !font1.loadFromFile("Fonts/Melted Monster.ttf")) {
            cout << "Failed to load fonts!" << endl;
            return;
        }

        // Instruction button 
        Texture instrsTexture;
        if (!instrsTexture.loadFromFile("Textures/i.png")) {
            cout << "Failed to load instructions icon!" << endl;
            exit(1);
        }
        Sprite instrsSprite(instrsTexture);
        instrsSprite.setScale(0.05f, 0.05f);
        instrsSprite.setPosition((45 * 20) - instrsSprite.getGlobalBounds().width - 80,
            (35 * 20) - instrsSprite.getGlobalBounds().height - 60);

        Text title("Hangman Game", font1, 50);
        title.setFillColor(Color::Black);
        title.setPosition(300, 200);

        Text opt1("Timed Mode", font1, 30);
        opt1.setFillColor(Color::Black);
        opt1.setPosition(240, 350);

        Text opt2("Untimed Mode", font1, 30);
        opt2.setFillColor(Color::Black);
        opt2.setPosition(480, 350);

        reset_game();

        // main game loop
        while (window.isOpen())
        {
            while (window.pollEvent(e))
            {
                handle_input(e, window, in_menu, in_game, in_end_screen, game_won, font, opt1, opt2, is_paused, instrsSprite);
            }

            window.clear(Color(128, 128, 128));

            if (in_menu) {
                draw_menu_background(window);

                //menu options and title
                window.draw(title);
                window.draw(opt1);
                window.draw(opt2);
                window.draw(instrsSprite); //instructions button
            }
            else if (in_game)
            {
                draw_background(window);
                draw_hangman(window);
                display_game_state(window, font1);
                draw_hint_button(window);

                if (is_game_won())
                {
                    game_won = true;
                    in_game = false;
                    in_end_screen = true;
                }
                else if (is_game_lost())
                {
                    game_won = false;
                    in_game = false;
                    in_end_screen = true;
                }
            }

            if (is_paused)
            {
                Text pause_text("The game is paused", font1, 50);
                pause_text.setFillColor(Color::Red);
                FloatRect textBounds = pause_text.getLocalBounds();
                pause_text.setPosition((window.getSize().x - textBounds.width) / 2,
                    window.getSize().y / 2 - textBounds.height);
                window.draw(pause_text);
            }

            if (in_end_screen)
            {
                endgame();
                window.close();
            }

            window.display();
        }
    }



    void render(RenderWindow& window) override
    {
        window.clear(Color(128, 128, 128));

        Font  f;
        if (!f.loadFromFile("Fonts/Melted Monster.ttf"))
        {
            cout << "Failed to load fonts in end game!" << endl;
            return;
        }

        if (timed)
        {
            draw_background(window);
            draw_hangman(window);
            display_game_state(window, f);
            draw_hint_button(window);
        }
        else
        {
            draw_menu_background(window);
        }

        if (is_paused)
        {

            Text pausedMessage("Game Paused", f, 50);
            pausedMessage.setFillColor(Color::White);
            pausedMessage.setPosition(window.getSize().x / 2 - pausedMessage.getLocalBounds().width / 2, window.getSize().y / 2 - 100);
            window.draw(pausedMessage);
        }

        window.display();
    }

    void endgame()
    {
        RenderWindow window(VideoMode(900, 700), "Game Over");

        Font font; // for win/lose message
        if (!font.loadFromFile("Fonts/Melted Monster.ttf")) {
            cout << "Failed to load font!" << endl;
            return;
        }

        Text mseg;
        mseg.setFont(font);
        mseg.setCharacterSize(50);
        mseg.setFillColor(Color::Black);

        string mainMessage;
        if (is_game_won())
        {
            mainMessage = "You Won!";
            mseg.setFillColor(Color(0, 100, 0));
            save_score();
            backgroundMusic.stop();    // Stop background music
            winSound.play();

        }
        else
        {
            mainMessage = "You Lost!";
            mseg.setFillColor(Color::Red);
            backgroundMusic.stop();    // Stop background music
            loseSound.play();

        }

        mseg.setString(mainMessage);


        Text wrdmsg;
        wrdmsg.setFont(font);
        wrdmsg.setCharacterSize(30);
        wrdmsg.setFillColor(Color::Black);
        wrdmsg.setString("The word was: " + string(get_saved_word())); // Use the saved word

        // position the win/lose message
        FloatRect textBounds = mseg.getLocalBounds();
        mseg.setPosition((window.getSize().x - textBounds.width) / 2, (window.getSize().y - textBounds.height) / 3);

        // position the word message
        FloatRect wordBounds = wrdmsg.getLocalBounds();
        wrdmsg.setPosition((window.getSize().x - wordBounds.width) / 2, (window.getSize().y - wordBounds.height) / 2 + 50);

        window.clear(Color(169, 169, 169));
        window.draw(mseg); // win/lose message
        window.draw(wrdmsg); // word message
        window.display();


        while (window.isOpen())
        {
            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                    window.close();
            }
        }
    }


    void process_guess(char guess)
    {

        for (int i = 0; i < guessed_wrd_count; i++)// seeing if the letter was already guessed
        {
            if (guessed_letters[i] == guess)
            {
                return;                         //if yes phir ignore
            }
        }


        guessed_letters[guessed_wrd_count++] = guess; // Add the guess to guessed_letters

        bool correct_guess = false;


        for (int i = 0; word[i] != '\0'; i++)
        {
            if (word[i] == guess)
            {
                guessed_word[i] = guess;
                correct_guess = true;
                setscore(getscore() + 1); //inc score
                update_score_text();

            }
        }


        if (!correct_guess)  //agar incorrect, reduce lives
        {
            lives--;
            /* loseLifeSound.play();*/
        }


    }



    void pause_timer()
    {
        if (!is_paused)
        {
            paused_time = game_clock.getElapsedTime();
            is_paused = true;
        }
    }

    void resume_timer()
    {
        if (is_paused)
        {
            total_elapsed_time += paused_time;
            game_clock.restart();
            is_paused = false;
        }
    }

    Time get_total_elapsed_time() const
    {
        return is_paused ? total_elapsed_time + paused_time : total_elapsed_time + game_clock.getElapsedTime();
    }
};