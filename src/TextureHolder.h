#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H
#include <SFML/Graphics.hpp>
#include <map>

using namespace std;
using namespace sf;

class TextureHolder {
private:
	map<string, Texture> textures;
	static TextureHolder* sInstance;

public:
	TextureHolder();
	static Texture& getTexture(string const& filename);
};
#endif