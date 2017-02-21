#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL\glew.h>

namespace rendering {
	class VertexBuffer {
	public:
		VertexBuffer(GLfloat* vertices, GLsizei size);
		~VertexBuffer();
		void bind();
		void unbind();
		inline GLuint getID() const { return m_VertexBufferID; }
		inline GLuint getSize() const { return m_VertexBufferSize; }
	private:
		GLuint m_VertexBufferID;
		GLuint m_VertexBufferSize;
	};
}

#endif