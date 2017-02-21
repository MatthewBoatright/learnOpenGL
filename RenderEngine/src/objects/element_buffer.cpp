#include "element_buffer.h"

namespace rendering {
	ElementBuffer::ElementBuffer(GLuint* indices, GLsizei size) {
		// Generate a new buffer to store our indices
		glGenBuffers(1, &m_ElementBufferID);
		// Make the new element buffer object active
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferID);
		// Upload the data to the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		// Unbind the buffer so we don't accidentally change the data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	ElementBuffer::~ElementBuffer() {
		// Delete the element buffer object
		glDeleteBuffers(1, &m_ElementBufferID);
	}

	void ElementBuffer::bind() {
		// Bind the element buffer object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferID);
	}

	void ElementBuffer::unbind() {
		// Unbind the element buffer object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}