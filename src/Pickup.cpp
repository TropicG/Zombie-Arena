#include "Pickup.h"

Pickup::Pickup(int type)
{
    //sets teh type of the pickup
    this->type = type;  

    //if the type is health pickup, we load the texture for health pickup 
    //and set the starting value for the pickup
    if (this->type == 1) {
        this->sprite.setTexture(TextureHolder::getTexture("graphics/health_pickup.png"));
        this->value = HEALTH_START_VALUE;
    }

    //if the type is ammo pickup, we load the texture for ammo pickup 
    //and set the starting value for the pickup
    else {
        this->sprite.setTexture(TextureHolder::getTexture("graphics/ammo_pickup.png"));
        this->value = AMMO_START_VALUE;
    }

    //setting the center of the image
    this->sprite.setOrigin(25, 25);

    //seconds that the pickup will be on the screen
    this->secondsToLive = START_SECONDS_TO_LIVE;

    //seconds the pickup will need to wait before spawining again
    this->secondsToWait = START_WAIT_TIME;
}

void Pickup::setArena(IntRect arena)
{
    //copies the details of the argument
    this->arena.left = arena.left + 50;
    this->arena.width = arena.width - 50;
    this->arena.top = arena.top + 50;
    this->arena.height = arena.height - 50;

    spawn();
}

void Pickup::spawn()
{
    srand((int)time(0) / this->type);
    int x = (rand() % this->arena.width);

    srand((int)time(0) * this->type);
    int y = (rand() % this->arena.height);

    this->secondsSinceSpawn = 0;
    this->spawned = true;

    this->sprite.setPosition(x, y);
}

FloatRect Pickup::getPosition()
{
    return this->sprite.getGlobalBounds();
}

Sprite Pickup::getSprite()
{
    return this->sprite;
}

void Pickup::update(float elapsedTime)
{
    if (this->spawned) {
        this->secondsSinceSpawn += elapsedTime;
    }

    else { 
        this->secondsSinceDeSpawn += elapsedTime;
    }


    if (this->secondsSinceSpawn > this->secondsToLive && this->spawned) {
        this->spawned = false;
        this->secondsSinceDeSpawn = 0;
    }

    if (this->secondsSinceDeSpawn > this->secondsToWait && !this->spawned) {
        this->spawn();
    }

}

bool Pickup::isSpawned()
{
    return this->spawned;
}

int Pickup::gotIt()
{
    this->spawned = false;
    this->secondsSinceDeSpawn = 0;
    return this->value;
}

void Pickup::upgrade()
{

    if (this->type == 1) { 
        this->value += (HEALTH_START_VALUE * .5);
    }
    else {
        this->value += (AMMO_START_VALUE * .5);
    }

    this->secondsToLive += (START_SECONDS_TO_LIVE / 10);
    this->secondsToWait -= (START_WAIT_TIME / 10);
}
