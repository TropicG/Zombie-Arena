#ifndef ZOMBIEARENA_H
#define ZOMBIEARENA_H
#include "Zombie.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int createBackground(VertexArray& rVA, IntRect arena);
Zombie* createHorde(int numZombies, IntRect arena);
#endif