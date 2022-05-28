#include "ZombieArena.h"

int createBackground(VertexArray& rVA, IntRect arena)
{

	//How big is the tile
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUADS = 4;

	//size of the arena in tiles
	int worldWidth = arena.width / TILE_SIZE;
	int worldHight = arena.height / TILE_SIZE;

	//setting the quads to be primitive type
	rVA.setPrimitiveType(Quads);

	//set the size of the vertex array
	rVA.resize(worldWidth * worldHight * VERTS_IN_QUADS);

	//start at the beginning at the vertex array
	int currentVertex = 0;

	for (int w = 0; w < worldWidth; ++w) {
		for (int h = 0; h < worldHight; ++h) {

			//setting the position for the current tile
			rVA[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
			rVA[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
			rVA[currentVertex + 3].position = Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			//checks if the tile is at the border of the arena
			if (h == 0 || h == worldHight - 1 || w == 0 || w == worldWidth - 1) {

				//we set the texCoordinates to the stone block in the sprite sheet
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_SIZE * TILE_TYPES);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else {

				//get random number every time
				srand((int)time(0) + h * w - h);

				//stands for Mud or grass, gets every time random tile
				int mOrG = (rand() % TILE_TYPES);

				//helps us in the calculating of the text coordinates
				int verticalOffSet = mOrG * TILE_SIZE;

				//setting the texCoords for the tile in the sprite sheet
				rVA[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffSet);
				rVA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffSet);
				rVA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffSet);
				rVA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffSet);
			}

			currentVertex += VERTS_IN_QUADS;
		}
	}

	return TILE_SIZE;
}

Zombie* createHorde(int numZombies, IntRect arena)
{
	Zombie* zombies = new Zombie[numZombies];

	int maxY = arena.height - 20, minY = arena.top + 20;	
	int maxX = arena.width - 20, minX = arena.left + 20;

	for (int i = 0; i < numZombies; ++i) {

		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;

		switch (side) {
		case 0:
			x = minX;
			y = (rand() % maxY) + minY;
			break;

		case 1:
			x = maxX;
			y = (rand() % maxY) + minY;
			break;

		case 2:
			y = maxY;
			x = (rand() % maxX) + minX;
			break;

		case 3:
			y = minY;
			x = (rand() % maxX) + minX;
			break;
		}

		srand((int)time(0) * i * 2);
		int type = (rand() % 3);
		zombies[i].spawn(x, y, type, i);
	}

	return zombies;
}
