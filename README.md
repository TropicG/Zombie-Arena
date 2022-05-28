# Zombie-Arena

Zombie Arena is a game where the player is chased by 3 different types of zombie and he has a gun and have to kill those zombies to advance the player. Every arena is randomly generated and pickups can be found spawning on the arena and the player has to get them before they respawn. The player have to keep eye for its remaining health or the zombies will eat his brain. I have made possible the ability to level up yours statas after every level.

What can you see in the project:
- The player using WASD to move and the Mouse to point in the screen and shoot
- Zombies chasing the player with different speed and health
- Health and ammo pickups spawning and despawing in the arena
- Ability to level up your fire rate, hp, ammo in clip and so on

How I structed the project:
- Player having a class and having a function to shoot with spawns a bullet
- Bullet having a class and calculating its trajectory and checking is there a collision with zombie
- Zombie having a class and checking has it colladed with bullet or the player
- Function that spawn the zombie and creating the arena using VertexArray
- Texture Holder class that stores the textures for the different sprites using map
