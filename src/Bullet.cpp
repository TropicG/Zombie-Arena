#include "Bullet.h"

Bullet::Bullet()
{
	this->bulletShape.setSize(sf::Vector2f(5,5));
}

void Bullet::stop()
{
	this->inFlight = false;
}

bool Bullet::isInFlight()
{
	return this->inFlight;
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	//keeps track of the bullet
	this->inFlight = true;
	this->position.x = startX;
	this->position.y = startY;

	//calculates the gradient of the flight path
	float gradient = (startX - targetX) / (startY - targetY);

	//if the gradient is negative we make it positive
	//since we want to have positive number so we can update the bullet each frame
	if (gradient <= 0) { 
		gradient *= -1;
	}


	//calculates the ratio between x and y
	float ratioXY = this->bulletSpeed / (1 + gradient);

	//set the "speed" horizontally and vertically
	this->bulletDistanceY = ratioXY;
	this->bulletDistanceX = ratioXY * gradient;

	//points the bullet in the right direction
	if (targetX < startX) { 
		this->bulletDistanceX *= -1;
	}
	if (targetY < startY) { 
		this->bulletDistanceY *= -1;
	}

	this->targetX = targetX;
	this->targetY = targetY;

	//setting the range for the bullet
	float range = 1000;
	this->minX = startX - range;
	this->maxX = startX + range;
	this->minY = startY - range;
	this->maxY = startY + range;

	//setting the position of the bullet
	this->bulletShape.setPosition(this->position);
}

FloatRect Bullet::getPosition()
{
	return this->bulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return this->bulletShape;
}

void Bullet::update(float elapsedTime)
{
	//sets the the position for the bullet based on the bulletDistance var we calculated
	//in shoot function
	this->position.x += (this->bulletDistanceX * elapsedTime);
	this->position.y += (this->bulletDistanceY * elapsedTime);

	//sets the position
	this->bulletShape.setPosition(this->position);

	//checks for collision
	if (this->position.x < this->minX || this->position.x > this->maxX ||
		this->position.y < this->minY || this->position.y > this->maxY) {

		this->stop();
	}
}
