#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <glad/glad.h>
#include <stb/stb_image.h>

class Texture {
public:
	Texture();
	Texture(const char* texPath);

	void bind();
	void destroy();

	~Texture();
private:
	unsigned int _id;
};

#endif
