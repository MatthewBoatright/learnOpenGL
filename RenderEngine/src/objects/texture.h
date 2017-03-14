#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL\glew.h>
#include <soil.h>

namespace rendering {
	class Texture {
	public:
		Texture(const char* textureFilePath);
		~Texture();
		void bind();
		void unbind();
		inline GLuint getID() const { return m_TextureID; }
	private:
		int m_width;
		int m_height;
		GLuint m_TextureID;
	};
}

#endif