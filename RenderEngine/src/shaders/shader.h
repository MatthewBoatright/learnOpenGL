#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include <GL/glew.h>

#include "../utils/fileUtils.h"

namespace rendering {
	class Shader {
	public:
		Shader(const GLchar* vertexFilePath, const GLchar* fragmentFilePath);
		void use();
		inline GLuint getProgram() const { return m_ProgramID; }
	private:
		GLuint m_ProgramID;
	};
}

#endif