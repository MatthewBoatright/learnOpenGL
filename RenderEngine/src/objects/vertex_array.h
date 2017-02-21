#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <GL\glew.h>

namespace rendering {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();
		void bind();
		void unbind();
		inline GLuint getID() const { return m_VertexArrayID; }
	private:
		GLuint m_VertexArrayID;
	};
}

#endif