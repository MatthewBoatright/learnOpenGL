#include "vertex_buffer.h"

namespace rendering {
	VertexBuffer::VertexBuffer(GLfloat* vertices, GLsizei size){
		// Generate a new buffer to store our vertices
		glGenBuffers(1, &m_VertexBufferID);
		// Make the new vertex buffer object active
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		// Upload the data to the buffer
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
		// Unbind the buffer so we don't accidentally change the data
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	VertexBuffer::~VertexBuffer() {
		// Delete the vertex buffer object
		glDeleteBuffers(1, &m_VertexBufferID);
	}

	void VertexBuffer::bind() {
		// Bind the vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void VertexBuffer::unbind() {
		// Unbind the vertex buffer object
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}