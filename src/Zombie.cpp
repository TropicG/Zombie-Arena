#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

bool Zombie::hit()
{
	this->health--;

	if (this->health <= 0) {

		this->alive = false;
		this->sprite.setTexture(TextureHolder::getTexture("graphics/blood.png"));
		return true;
	}

	return false;
}

bool Zombie::isAlive()
{
	return this->alive;
}

void Zombie::spawn(float startX, float startY, int type, int seed)
{

	switch (type) {
	case 0:
		//Bloater
		this->sprite = Sprite(TextureHolder::getTexture("graphics/bloater.png"));

		this->speed = BLOATER_SPEED;
		this->health = BLOATER_HEALTH;
		break;

	case 1:
		//Chaser
		this->sprite = Sprite(TextureHolder::getTexture("graphics/chaser.png"));

		this->speed = CHASER_SPEED;
		this->health = CHASER_HEALTH;
		break;

	case 2:
		//Crawler
		this->sprite = Sprite(TextureHolder::getTexture("graphics/crawler.png"));

		this->speed = CRAWLER_SPEED;
		this->health = CRAWLER_HEALTH;
		break;
	}

	//Modifying the speed to make every zombie unique
	//Every zombie is unique. Create a speed modifier
	srand((int)time(0) * seed);

	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;

	modifier /= 100;
	this->speed *= modifier;

	//setting the position of the zombie
	this->position.x = startX;
	this->position.y = startY;

	//setting the origin of the sprite to be its center
	this->sprite.setOrigin(25, 25);

	//setting the position of the sprite
	this->sprite.setPosition(position);
}

FloatRect Zombie::getPosition()
{
	return this->sprite.getGlobalBounds();
}

Sprite Zombie::getSprite()
{
	return this->sprite;
}

void Zombie::update(float elapsedTime, Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	if (playerX > this->position.x) {
		this->position.x += (this->speed * elapsedTime);
	}
	if (playerX < this->position.x) {
		this->position.x -= (this->speed * elapsedTime);
	}
	if (playerY > this->position.y) {
		this->position.y += (this->speed * elapsedTime);
	}
	if (playerY < this->position.y) {
		this->position.y -= (this->speed * elapsedTime);
	}

	this->sprite.setPosition(this->position);

	//float angle = (atan2 (playerY - this->position.y, playerX - this->position.x) * 180) / 3.141;
	float angle = (atan2(playerY - this->position.y, playerX - this->position.x));
	angle *= 180;
	angle /= 3.141;

	this->sprite.setRotation(angle);
}
