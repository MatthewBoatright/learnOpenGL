#include "vertex_array.h"

namespace rendering {
	VertexArray::VertexArray() {
		// Generate the vertex array object
		glGenVertexArrays(1, &m_VertexArrayID);
	}

	VertexArray::~VertexArray() {
		// Delete the vertex array object
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void VertexArray::bind() {
		// Bind the vertex array object
		glBindVertexArray(m_VertexArrayID);
	}

	void VertexArray::unbind() {
		// Unbind the vertex array object
		glBindVertexArray(0);
	}
}