//writing centipede code myself

#include<iostream>
#include<cstdlib>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace std;

//initiliazing dimensions
const int resolutionX = 960;
const int resolutionY = 960;
const int boxpixelsX = 32;
const int boxpixelsY = 32;
const int gameRows = resolutionX / boxpixelsX;
const int gameColumns = resolutionY / boxpixelsY;

//initliazing game grid
int gameGrid[gameRows][gameColumns] = {};

//for readibility
const int x = 0;
const int y = 1;
const int exists = 2;
//function declarations
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void inputPlayer(float player[], float bullet[], sf::Clock &playerClock);
void moveBullet(float bullet[], sf::Clock& bulletClock);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void drawCentipede(sf::RenderWindow& window, float centipede[4][12], int i, sf::Sprite& headcentipedeSprite, sf::Sprite& bodycentipedeSprite);
void moveCentipede(float centipede[5][12], float mushrooms[5][100], int count1, sf::Clock& centipedeClock);
void placeMushrooms(float mushrooms[5][100], int count1);
void drawMushrooms(sf::RenderWindow& window, float mushrooms[5][100], int count1, sf::Sprite& mushroomsSprite);
void addmushroom(float mushrooms[5][100], int count1, int a, int b);
//void collisionMushrooms(float bullet[], float mushrooms[5][30], int count1);

int main()
{
	srand(time(0));
	bool centiMode = false;
	//Declaring Renderwindow
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);
	//Resizing the window
	window.setSize(sf::Vector2u(640, 640));

	//Used to position your window on every launch
	window.setPosition(sf::Vector2i(100, 0));

	//Initializing Background music
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/august.mp3");
	bgMusic.play();
	bgMusic.setVolume(50);

	//Initializing Background
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/background.png");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); //Opacity reduced to 25%

	//Initializing Player and Player Sprites
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxpixelsX;
	player[y] = (gameColumns * 3 / 4) * boxpixelsY;
	sf::Clock playerClock;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxpixelsX, boxpixelsY));

	//Initializing Bullet and Bullet Sprites
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxpixelsY;
	bullet[exists] = true;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxpixelsX, boxpixelsY));

	//Initializing Centipede
	float centipede[5][12] = {};
	int x_box = (rand() % 8 + 1);
	int y_box = (rand() % 8 + 1);
	centipede[x][0] = x_box * boxpixelsX;
	centipede[y][0] = y_box * boxpixelsY;
	centipede[exists][0] = true;
	centipede[3][0] = 0; //forward
	centipede[4][0] = true;
	for (int i = 1; i < 12; ++i) {
		centipede[x][i] = x_box * boxpixelsX;
		centipede[y][i] = y_box * boxpixelsY;
		centipede[exists][i] = true;
		centipede[3][i] = 0; //forward
		centipede[4][i] = 0; //ishead
	}
	for (int i = 1; i < 12; ++i) {
		centipede[x][i] = centipede[x][i - 1] + 32;
		centipede[exists][i] = true;
	}
	sf::Clock centipedeClock;
	sf::Texture headcentipedeTexture;
	sf::Texture bodycentipedeTexture;
	sf::Sprite headcentipedeSprite;
	sf::Sprite bodycentipedeSprite;
	headcentipedeTexture.loadFromFile("Textures/c_head_left_walk.png");
	headcentipedeSprite.setTexture(headcentipedeTexture);
	bodycentipedeTexture.loadFromFile("Textures/c_body_left_walk.png");
	bodycentipedeSprite.setTexture(bodycentipedeTexture);
	headcentipedeSprite.setTextureRect(sf::IntRect(0, 0, 28, boxpixelsY));
	bodycentipedeSprite.setTextureRect(sf::IntRect(0, 0, 28, boxpixelsY));

	//Initializing Mushrooms
	float mushrooms[5][100] = {};
	mushrooms[x][0];
	mushrooms[y][0];
	mushrooms[exists][0] = true;
	mushrooms[4][0]; //hits
	bool mush[30] = { false };
	sf::Clock mushroomsClock;
	sf::Texture mushroomsTexture;
	sf::Sprite mushroomsSprite;
	mushroomsTexture.loadFromFile("Textures/mushroom.png");
	mushroomsSprite.setTexture(mushroomsTexture);
	sf::IntRect mushroomsrect(0, 0, boxpixelsX, boxpixelsY);
	mushroomsSprite.setTextureRect(mushroomsrect);
	int count1 = rand() % 11 + 20;
	int r = 0, c = 0;
	for (int i = 0; i < count1; ++i) {
		mushrooms[4][i] = 0;
	}
	placeMushrooms(mushrooms, count1);


	while (window.isOpen()) {
		//Calling functions
		window.draw(backgroundSprite);
		drawPlayer(window, player, playerSprite); //drawing the player sprite

		//drawing centipede
		for (int i = 0; i < 12; ++i) { //to move every part of the centipede
				drawCentipede(window, centipede, i, headcentipedeSprite, bodycentipedeSprite);
				moveCentipede(centipede, mushrooms, count1, centipedeClock);
		}

		drawMushrooms(window, mushrooms, count1, mushroomsSprite); //drawing the mushrooms

		//drawing bullet
		if (bullet[exists] == true) { //to  make sure there is one bullet at a time
			moveBullet(bullet, bulletClock);
			drawBullet(window, bullet, bulletSprite);
		}
		/*for (int i = 0; i < count1; ++i) {
			int pointx = mushrooms[0][i] * 32;
			int pointy = mushrooms[1][i] * 32;
			if (mushrooms[exists][i]) {
				if (bullet[x] >= pointx - 10 && bullet[x] <= pointx + 10 && bullet[y] >= pointy - 10 && bullet[y] <= pointy + 10) {
					mushrooms[exists][i] = false;
					bullet[exists] = false;
				}
			}
		}*/

		//mushroom collision with the bullet
		
		for (int i = 0; i < count1; ++i) {
			int pointx = mushrooms[0][i] * 32;
			int pointy = mushrooms[1][i] * 32;
			if (mushrooms[exists][i ]&& bullet[exists]) {
				if (bullet[x] >= pointx - 10 && bullet[x] <= pointx + 10 && bullet[y] >= pointy - 10 && bullet[y] <= pointy + 10) {
					mushrooms[4][i] += 1;
					bullet[exists] = false;
					//mush[i] = false;
					/*for (int j = 0; j < 3; ++j) {
						if (!mush[i] && mushroomsClock.getElapsedTime().asMilliseconds() > 2.5) {
							mushroomsrect.left += 32;
							mushroomsSprite.setTextureRect(mushroomsrect);
							mushroomsClock.restart();
							mush[i] = true;
						}
					}*/
					if (mushrooms[4][i] == 2) {
						mushrooms[exists][i] = false;
						bullet[exists] = false;
					}
				}
			}
		}	

		//centipede collision
		for (int i = 0; i < 12; ++i) {
			int gridx = centipede[x][i];
			int gridy = centipede[y][i];
			if (bullet[x] >= gridx - 10 && bullet[x] <= gridx + 10 && bullet[y] >= gridy - 10 && bullet[y] <= gridy + 10) {
				centipede[exists][i] = false;
				if (bullet[exists]) {
					addmushroom(mushrooms, count1, gridx, gridy);
				}
				bullet[exists] = false;
			}
		}

		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				return 0;
			}
		}

		//taking inputs from the player
		inputPlayer(player, bullet, playerClock);

		window.display();
		window.clear();

	}
}
//Function Definitions
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

void inputPlayer(float player[], float bullet[], sf::Clock& playerClock) {
	if (playerClock.getElapsedTime().asMilliseconds() < 70)
		return;

	playerClock.restart();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (player[x] < 928) {
			player[x] += 32;
			player[y] += 0;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (player[x] > 0) {
			player[x] -= 32;
			player[y] += 0;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (player[y] >= (gameColumns * 3 / 4) * boxpixelsY) {
			player[x] += 0;
			player[y] -= 32;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (player[y] < 928) {
			player[x] += 0;
			player[y] += 32;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (bullet[exists] == false) {
			bullet[x] = player[x];
			bullet[y] = player[y] - boxpixelsY;
			bullet[exists] = true;
		}
	}
}

void moveBullet(float bullet[], sf::Clock& bulletClock) {
	if (bulletClock.getElapsedTime().asMilliseconds() < 6.5)
		return;

	bulletClock.restart();
	bullet[y] -= 10;
	if (bullet[y] < -32)
		bullet[exists] = false;
}

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

void drawCentipede(sf::RenderWindow& window, float centipede[4][12], int i, sf::Sprite& headcentipedeSprite, sf::Sprite& bodycentipedeSprite) {
	if (centipede[exists][i]) {
		if (centipede[4][i] == true) {
			headcentipedeSprite.setPosition(centipede[x][i], centipede[y][i]);
			window.draw(headcentipedeSprite);
		}
		else {
			bodycentipedeSprite.setPosition(centipede[x][i], centipede[y][i]);
			window.draw(bodycentipedeSprite);
		}
	}
}

void moveCentipede(float centipede[5][12], float mushrooms[5][100], int count1, sf::Clock& centipedeClock) {
	if (centipedeClock.getElapsedTime().asMilliseconds() > 70) {
		for (int i = 0; i < 12; ++i) {
			if (centipede[exists][i]) {
					if (centipede[3][i] == 1) {
						centipede[x][i] += 32;
						for (int j = 0; j < count1; ++j) {
							if (mushrooms[exists][j]) {
								if (mushrooms[0][j] * 32 == centipede[x][i] && mushrooms[1][j] * 32 == centipede[y][i]) {
									//mushrooms[exists][j] = false;
									centipede[3][i] = 0;
									centipede[y][i] += 32;
								}
							}
						}
						if (centipede[x][i] >= 928) {
							centipede[3][i] = 0;
							centipede[y][i] += 32;
						}
					}
					else if (centipede[3][i] == 0) {
						centipede[x][i] -= 32;
						for (int j = 0; j < count1; ++j) {
							if (mushrooms[exists][j]) {
								if (mushrooms[0][j] * 32 == centipede[x][i] && mushrooms[1][j] * 32 == centipede[y][i]) {
									//mushrooms[exists][j] = false;
									centipede[3][i] = 1;
									centipede[y][i] += 32;
								}
							}
						}
						if (centipede[x][i] < 0) {
							centipede[3][i] = 1;
							centipede[y][i] += 32;
						}
					}
				
					if (centipede[y][i] > 928) {
						centipede[y][i] = 740;
						/*centipede[exists][i] = false;
						if (centipede[x][i] < 0) {
							centipede[3][i] = 1;
							centipede[y][i] -= 32;
						}
						if (centipede[x][i] >= 928) {
							centipede[3][i] = 0;
							centipede[y][i] -= 32;
						}*/
					}
				
			}
		}
		centipedeClock.restart();
	}
}

void placeMushrooms(float mushrooms[5][100], int count1) {
	for (int i = 0; i < count1; ++i) {
		int r = rand() % 28;
		int c = rand() % 30;
		mushrooms[0][i] = r;
		mushrooms[1][i] = c;
		mushrooms[exists][i] = true;
	}

}

void drawMushrooms(sf::RenderWindow& window, float mushrooms[5][100], int count1, sf::Sprite& mushroomsSprite) {
	for (int i = 0; i < count1; ++i) {
		if (mushrooms[exists][i] == true) {
			//if (i == count1 - 1) cout << mushrooms[0][i] << ' ' << mushrooms[1][i] << endl;
			mushroomsSprite.setPosition(mushrooms[0][i]*32, mushrooms[1][i]*32);
			window.draw(mushroomsSprite);
		}
	}
}

void addmushroom(float mushrooms[5][100], int count1, int a, int b) {
	++count1;
	mushrooms[0][count1 - 1] = a/32;
	mushrooms[1][count1 - 1] = b/32;
	mushrooms[exists][count1 - 1] = true;
}












