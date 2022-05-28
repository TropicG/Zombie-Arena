#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <fstream>
#include "Player.h"
#include "ZombieArena.h"
#include "Bullet.h"
#include "Pickup.h"
#include "TextureHolder.h"


const int MAX_WIDTH = 1000;
const int MAX_HEIGHT = 1000;


using namespace sf;
	
int main() {

	TextureHolder holder;

	//the game will always be in one of these four states
	enum class State{PAUSED, LEVELING_UP, GAME_OVER, PLAYING};

	//starting the game in game over state
	State state = State::GAME_OVER;

	//getting the screen resolution and creating a SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);

	//Create a SMFL View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//Our clock for timing everything
	Clock clock;

	//How long has the PLAYING state active
	Time gameTotalTime;

	//Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;

	//Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	//creating the player
	Player player;

	//the boundaries of the arena
	IntRect arena;

	//creating the background
	VertexArray background;
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background_sheet.png");
	
	//the number of zombies for each arena
	int numZombies = 0;
	
	//the number of alive zombies for each arena
	int numZombiesAlive = 0;

	//dyn array that will hold the zombies for each arena
	Zombie* zombies = nullptr;

	//vars to keep us in check how many  bullet we have and what is the fire rate
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletSpare = 24;
	int bulletInClip = 6;
	int clipSize = 6;
	float fireRate = 1.5;

	//when was the last time we fired a bullet
	Time lastPressed;

	//setting the cursor visible
	window.setMouseCursorVisible(false);

	//creating the sprite for the crosshair and getting its texture
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::getTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	//creating the pickups in the arena
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	//scores about the game
	int score = 0;
	int hiScore = 0;

	//text for game over
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::getTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	//creating a view for the hud
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//creating the ammo icon 
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::getTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	//setting the font of the game
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	//creating the text, setting its size, color, position when we paused the game
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");

	//creating the text, setting its size, color, position for the game over screen
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	//creating the text for the level up section
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);

	//making the text for the choices
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	//text for the ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	//text for the score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	std::ifstream inputFile("gamedata/score.txt");
	if (inputFile.is_open()) {
		inputFile >> hiScore;
		inputFile.close();
	}


	//text for the highscore
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	//zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	//current wave
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	//healthbar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	// Debug HUD
	Text debugText;
	debugText.setFont(font);
	debugText.setCharacterSize(25);
	debugText.setFillColor(Color::White);
	debugText.setPosition(20, 220);
	std::ostringstream ss;

	//when we last update the HUD
	int framesSinceLastHUDUpdate = 0;

	//How often we should update the HUD
	int fpsMeasurementFrameInterval = 1000;

	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/splat.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);

	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);


	while (window.isOpen()) {

		Event event;
		while (window.pollEvent(event)) {

			//Pause a game while playing
			if (event.key.code == Keyboard::Return && state == State::PLAYING) {
				state = State::PAUSED;
			}

			//Restarting while paused
			else if (event.key.code == Keyboard::Return && state == State::PAUSED) {

				state = State::PLAYING;

				//restarting the clock so there is no frame jump
				clock.restart();
			}

			else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {

				state = State::LEVELING_UP;

				wave = 0;
				score = 0;

				currentBullet = 0;
				bulletSpare = 24;
				bulletInClip = 6;
				clipSize = 6;
				fireRate = 1.5;

				player.resetPlayerStats();
			}

			if (state == State::PLAYING) {
				
				//reloading
				if (event.key.code == Keyboard::R) {

					//if there are plenty of bullets
					if (bulletSpare >= clipSize) {

						while (bulletInClip != clipSize) {
							bulletInClip++;
							bulletSpare--;
						}

						/*
						//reload to the clip
						bulletInClip = clipSize;
						
						//subtract the clipsize from the spare bullets
						bulletSpare -= clipSize;
						
						*/

						reload.play();
					}
					else if (bulletSpare > 0 && bulletInClip <= clipSize) {

						//if there are a few bullets we put them in the clip
						bulletInClip = bulletSpare;
						bulletSpare = 0;

						reload.play();
					}
					else {
						reloadFailed.play();
					}
				}
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
	

		//Handeling the WASD input
		if (state == State::PLAYING) {

			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else {
				player.stopUp(); 
			}

			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else {
				player.stopRight();
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else { 
				player.stopDown(); 
			}

			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else { 
				player.stopLeft();
			}

			//if the player fires a bullet
			if (Mouse::isButtonPressed(sf::Mouse::Left)) {

				if (gameTotalTime.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletInClip > 0) {

					//passes the center of the player and the center to the crosshair to the  shoot function
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;
					if (currentBullet > 99) {
						currentBullet = 0;
					}

					lastPressed = gameTotalTime;

					shoot.play();

					bulletInClip--;
				}
			}
		}

		//Handeling the LEVELING UP state
		if (state == State::LEVELING_UP) {

			//Handle the player leveling up
			if (event.key.code == Keyboard::Num1) {
				fireRate++;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2) {
				clipSize += clipSize;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3) {
				player.upgradeHealth();	
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4) {
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5) {
				healthPickup.upgrade();	
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6) {
				ammoPickup.upgrade();
				state = State::PLAYING;
			}
	
			if (state == State::PLAYING) {

				wave++;

				//Preparing the level
				arena.width = 340 * (wave) ;
				arena.height = 340 * (wave);
				arena.left = 0;
				arena.top = 0;

				if (arena.width > MAX_WIDTH) {
					arena.widht = MAX_WIDTH;
				}
				if (arena.height > MAX_HEIGHT) {
					arena.height = MAX_HEIGHT;
				}

				//gets the size of the tile
				int tileSize = createBackground(background, arena);

				//spawns the player at the middle of the arena
				player.spawn(arena, resolution, tileSize);

				//passes the arena to the pickups so they can spawn
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//number of zombies
				numZombies = 3 * wave;

				//freeing up allocated memory so that we create a new horde
				delete[] zombies;

				//creating the new horde
				zombies = createHorde(numZombies, arena);

				//the number of alive zombies for now is the number of created one 
				numZombiesAlive = numZombies;

				powerup.play();

				//restarting the clock so there are not any frame jumps
				clock.restart();
			}

		}

		if (state == State::PLAYING) {
			
			//update the delta time
			Time dt = clock.restart();

			//update the total game time
			gameTotalTime += dt;

			//make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			//Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			//convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			//sets the crosshair to the mouse world position
			spriteCrosshair.setPosition(mouseWorldPosition);

			//Update the player
			player.update(dt.asSeconds(), Mouse::getPosition());

			//Make a note of the player new position
			Vector2f playerPosition(player.getCenter());

			//make the view center around the player
			mainView.setCenter(player.getCenter());

			//updating the zombies
			for (int i = 0; i < numZombies; ++i) {

				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			//updatting the bullets
			for (int i = 0; i < 100; ++i) {

				if (bullets[i].isInFlight()) {
					bullets[i].update(dt.asSeconds());
				}
			}

			//updates the pickups
			healthPickup.update(dt.asSeconds());
			ammoPickup.update(dt.asSeconds());


			//checks does each bullet interscects with a alive zombie
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numZombies; j++) {

					if (bullets[i].isInFlight() && zombies[j].isAlive()) {

						//if we hit a zombie with a bullet
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {

							//we stop the bullet
							bullets[i].stop();

							//checks if we killed the zombie
							if (zombies[j].hit()) {

								//if we did we increase the score
								score += 10;
								if (score >= hiScore) {
									hiScore = score; 
								}
								
								//and we subtract one zombie
								numZombiesAlive--;

								//if we killed all zombies in the arena we go to the leveling up

								splat.play();

								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}

							}

						}

					}

				}
			}


			

			for (int i = 0; i < numZombies; ++i) {

				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()) {

					if (player.hit(gameTotalTime)) {

						hit.play();
					}

					if (player.getHealth() <= 0) {
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			}

			//if the player collides with the pickup we increase the health
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				pickup.play();
				player.increaseHealthLevel(healthPickup.gotIt());
			}

			//if the player g
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				pickup.play();
				bulletSpare += ammoPickup.gotIt();
			}

			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));

			framesSinceLastHUDUpdate++;

			std::stringstream ssAmmo;
			std::stringstream ssZombiesAlive;

			ssAmmo << bulletInClip << "/" << bulletSpare;
			ammoText.setString(ssAmmo.str());

			ssZombiesAlive << "Zombies:" << numZombiesAlive;
			zombiesRemainingText.setString(ssZombiesAlive.str());


			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {

				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;

				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				ssHiScore << "Hi score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				framesSinceLastHUDUpdate = 0;
			}

		}

		if (state == State::PLAYING) {

			window.clear();

			window.setView(mainView);

			window.draw(background, &textureBackground);

			for (int i = 0; i < numZombies; ++i) {
				window.draw(zombies[i].getSprite());
			}


			for (int i = 0; i < 100; ++i) {

				if (bullets[i].isInFlight()) {
					window.draw(bullets[i].getShape());
				}
			}

			window.draw(player.getSprite());

			if (ammoPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}


			if (healthPickup.isSpawned()) {
				window.draw(healthPickup.getSprite());
			}

			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);

			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
				
		}

		if (state == State::LEVELING_UP) {

			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED) {
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER) {

			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();
	}

	//preventing memory allocation
	delete[] zombies;

	return 0;
}