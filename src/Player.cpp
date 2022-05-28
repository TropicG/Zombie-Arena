#include "Player.h"
#include "TextureHolder.h"

Player::Player()
{
	this->speed = this->START_SPEED;
	this->health = this->START_HEALTH;
	this->maxHealth = this->START_HEALTH;

	this->sprite = Sprite(TextureHolder::getTexture("graphics/player.png"));

	//sets the origin of the sprite to the center, for smoother rotation
	this->sprite.setOrigin(25, 25);
}

void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	//place the player into the middle in the arena
	this->position.x = arena.width / 2;
	this->position.y = arena.height / 2;

	//copy the details of the arena to the players arena
	this->arena.left = arena.left;
	this->arena.width = arena.width;
	this->arena.top = arena.top;
	this->arena.height = arena.height;

	//remember how big are the tiles in the this area
	this->tilesSize = tileSize;

	//store the resolution for further use
	this->resolution.x = resolution.x;
	this->resolution.y = resolution.y;
}

void Player::resetPlayerStats()
{
	this->speed = START_SPEED;
	this->health = START_HEALTH;
	this->maxHealth = START_HEALTH;	
}

Time Player::getLastHitTime()
{
	return this->lastHit;
}

FloatRect Player::getPosition()
{
	return this->sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return this->position;
}

float Player::getRotation()
{
	return this->sprite.getRotation();
}

Sprite Player::getSprite()
{
	return this->sprite;
}

void Player::moveLeft()
{
	this->leftPressed = true;
}

void Player::moveRight()
{
	this->rightPressed = true;
}

void Player::moveUp()
{
	this->upPressed = true;
}

void Player::moveDown()
{
	this->downPressed = true;
}

void Player::stopLeft()
{
	this->leftPressed = false;
}

void Player::stopRight()
{
	this->rightPressed = false;
}

void Player::stopUp()
{
	this->upPressed = false;
}

void Player::stopDown()
{
	this->downPressed = false;
}

void Player::update(float elapsedTime, Vector2i mousePosition)
{
	if (this->upPressed) {
		this->position.y -= this->speed * elapsedTime;
	};
	if (this->downPressed) {
		this->position.y += this->speed * elapsedTime;
	}
	if (this->leftPressed) {
		this->position.x -= this->speed * elapsedTime;
	}
	if (this->rightPressed) {
		this->position.x += this->speed * elapsedTime;
	}

	this->sprite.setPosition(this->position);

	if (this->position.x > this->arena.width - this->tilesSize) {
		this->position.x = this->arena.width - this->tilesSize;
	}

	if (this->position.x < this->arena.left + this->tilesSize) {
		this->position.x = this->arena.left + this->tilesSize;
	}

	if (this->position.y > this->arena.height - this->tilesSize) {
		this->position.y = this->arena.height - this->tilesSize;
	}

	if (this->position.y < this->arena.top + this->tilesSize) {
		this->position.y = this->arena.top + this->tilesSize;
	}

	float angle = (atan2(mousePosition.y - this->resolution.y / 2,
		mousePosition.x - this->resolution.x / 2));
	angle *= 180;
	angle /= 3.141;

	this->sprite.setRotation(angle);
}

void Player::upgradeSpeed()
{
	this->speed += (this->START_SPEED * .2);
}

void Player::upgradeHealth()
{
	this->maxHealth += (this->START_HEALTH * .2);
}

void Player::increaseHealthLevel(int amount)
{
	this->health += amount;

	if (this->health > this->maxHealth) {
		this->health = this->maxHealth;
	}
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - this->lastHit.asMilliseconds() > 200) {
		this->lastHit = timeHit;
		this->health -= 10;
		return true;
	}

	return false;
}

int Player::getHealth()
{
	return this->health;
}
