#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::sInstance = nullptr;

TextureHolder::TextureHolder()
{
	assert(this->sInstance == nullptr);
	this->sInstance = this;
}

Texture& TextureHolder::getTexture(string const& filename)
{
	//gets a reference to textures using sInstance
	auto& m = sInstance->textures;

	//creates an iterator to hold a key-value-pair (kvp)
	//and searches for the required kvp
	//using the passed in the filename
	auto keyValuePair = m.find(filename);

	//if we have found a match we return the texture 
	if (keyValuePair != m.end()) {
		return keyValuePair->second;
	}

	else {
		//filename not found 
		//creating a new key value pair using filename
		auto& texture = m[filename];

		//load the texture from file in the usual way
		texture.loadFromFile(filename);

		//return the texture to the calling code
		return texture;
	}
}
