#ifndef PLAYER_H
#define PLAYER_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	//position of the player
	Vector2f position;

	//sprite of the player
	Sprite sprite;

	//texture of the player
	Texture texture;

	//what is the resolution
	Vector2f resolution;

	//What size is the current arena
	IntRect arena;

	//how big is each tile of the arena
	int tilesSize;

	//which direction is the player currently moving in
	bool upPressed;
	bool downPressed;
	bool leftPressed;
	bool rightPressed;

	//the current healt and the max health the player has
	int health;
	int maxHealth;

	//when was the player last hit
	Time lastHit;

	//speed in pixels per hour
	float speed;

public:
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	//call this at the end of every game
	void resetPlayerStats();

	//handle the player getting hit by a zombie
	bool hit(Time timeHit);

	//how long was the player last hit
	Time getLastHitTime();

	//where is the player
	FloatRect getPosition();

	//where is the center of the player
	Vector2f getCenter();

	//what angle is the player facing
	float getRotation();

	//getting the sprite 
	Sprite getSprite();

	//these four functions moves the player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	//these four funcions stops the player
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	//updating the player every frame
	void update(float elapsedTime, Vector2i mousePosition);

	//give the player a speed boost
	void upgradeSpeed();

	//give the player some health
	void upgradeHealth();

	//increase the maximum amount of health the player can have
	void increaseHealthLevel(int amount);

	//how much health the player currently has
	int getHealth();
};
#endif
