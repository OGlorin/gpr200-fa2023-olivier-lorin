#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"

unsigned int loadTexture(const char* filePath, int wrapMode, int minFilter, int magFilter) {

	stbi_set_flip_vertically_on_load(true);

	int width, height, numComponents;
	unsigned char* data = stbi_load(filePath, &width, &height, &numComponents, 0);
	if (data == NULL) {
		printf("Failed to load image %s", filePath);
		stbi_image_free(data);
		return 0;
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	GLenum storage;
	switch (numComponents)
	{
	case 1:
		storage = GL_RED;
		break;
	case 2:
		storage = GL_RG;
		break;
	case 3:
		storage = GL_RGB;
		break;
	case 4:
		storage = GL_RGBA;
		break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, storage, width, height, 0, storage, GL_UNSIGNED_BYTE, data);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	return texture;
}