#ifndef ELEMENT_BUFFER_H
#define ELEMENT_BUFFER_H

#include <GL/glew.h>

namespace rendering {
	class ElementBuffer {
	public:
		ElementBuffer(GLuint* indices, GLsizei size);
		~ElementBuffer();
		void bind();
		void unbind();
		inline GLuint getID() const { return m_ElementBufferID; }
	private:
		GLuint m_ElementBufferID;
	};
}

#endif