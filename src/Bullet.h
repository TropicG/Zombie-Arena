#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet {
private:

	//position of the bullet
	Vector2f position;

	//shape of the bullet
	RectangleShape bulletShape; 

	//is the bullet traveling?
	bool inFlight;

	//speed of the bullet
	float bulletSpeed = 1000;
	
	//how much the bullet travels each frame
	float bulletDistanceX;
	float bulletDistanceY;

	float targetX;
	float targetY;

	//these vars are used for collision detection
	float maxX;
	float maxY; 
	float minX; 
	float minY; 

public:
	Bullet();

	//stopping the bullet
	void stop();

	//returns wheter the bullet is in the air or not
	bool isInFlight();

	//launches the bullet
	void shoot(float startX, float startY, float targetX, float targetY);

	//gets the position
	FloatRect getPosition();

	//gets the shape
	RectangleShape getShape();

	//updates the bullet each frame
	void update(float elapsedTime);
};
#endif