#ifndef PICKUP_H
#define PICKUP_H
#include <SFML\Graphics.hpp>
#include "TextureHolder.h"

using namespace sf;

class Pickup {
private:

	//sets the starting value for all health pickups
	const int HEALTH_START_VALUE = 50;

	//sets the starting value for all ammo pickups
	const int AMMO_START_VALUE = 12;

	//how many seconds a pickup will wait before dissapear
	const int START_WAIT_TIME = 10;

	//how many seconds a picup will last between spawns
	const int START_SECONDS_TO_LIVE = 5;

	//sprite of the pickup
	Sprite sprite;

	//size of the arena
	IntRect arena;

	//how much is the ammo or pickup worth
	int value;

	//what is the type of the picup
	//1 - health
	//2 - ammo
	int type;

	//is the pickup spawned
	bool spawned;

	//seconds since the spawn of the pickup
	float secondsSinceSpawn;

	//seconds since the despawn of the pickup
	float secondsSinceDeSpawn;

	//how long the pickup will stay before disapearing
	float secondsToLive;

	//how long the pickup stays despawned
	float secondsToWait;
public:

	//Used to initialise the type of the pickup
	Pickup(int type);

	//prepares a new pickup
	//will be called for each Pickup object in the arena
	void setArena(IntRect arena);

	//handeling spawing the pickup
	void spawn();

	//returns the position of the pickup
	FloatRect getPosition();

	//returns the sprite of the pickup
	Sprite getSprite();


	void update(float elapsedTime);
	
	bool isSpawned();
	
	int gotIt();

	void upgrade();
};
#endif