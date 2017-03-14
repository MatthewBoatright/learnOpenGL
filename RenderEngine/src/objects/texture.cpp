#include "texture.h"

namespace rendering {

	Texture::Texture(const char* textureFilePath) {
		unsigned char* image = SOIL_load_image(textureFilePath, &m_width, &m_height, 0, SOIL_LOAD_RGB);

		glGenTextures(1, &m_TextureID);
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		unbind();
	}

	Texture::~Texture() {

	}

	void Texture::bind() {
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Texture::unbind() {
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}
}