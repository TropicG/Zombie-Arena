#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie {
private:
	//the speed of the different zombies
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	//the toughness of the different zombies
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	//make each zombie vary its speed slightly 
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	//position of the zombie
	Vector2f position;

	//sprite of the zombie
	Sprite sprite;

	//current speed of the zombie
	float speed = 0;

	//current health of the zombie
	float health = 0;

	//is the zombie alive
	bool alive;

public:

	//handles when a bullet hits a zombie
	bool hit();

	//checks is it alive
	bool isAlive();

	//spawns the zombie
	void spawn(float startX, float startY, int type, int seed);

	//gets the position of the zombie
	FloatRect getPosition();

	//gets teh spite of the zombie
	Sprite getSprite();

	//updates the zombies each frame
	void update(float elapsedTime, Vector2f playerLocation);
};
#endif