#include "shader.h"

using namespace fileUtilities;

namespace rendering {
	Shader::Shader(const GLchar* vertexFilePath, const GLchar* fragmentFilePath) {
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Read Shaders
		std::string vertexShaderStr = getFileContents(vertexFilePath);
		std::string fragmentShaderStr = getFileContents(fragmentFilePath);
		const char* vertexShaderSrc = vertexShaderStr.c_str();
		const char* fragmentShaderSrc = fragmentShaderStr.c_str();

		GLint result = GL_FALSE;
		int logLength;

		// Compile vertex shader
		std::cout << "Compiling vertex shader..." << std::endl;
		glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
		glCompileShader(vertexShader);

		// Check vertex shader
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> vertexShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(vertexShader, logLength, NULL, &vertexShaderError[0]);
		std::cout << &vertexShaderError[0] << std::endl;

		// Compile fragment shader
		std::cout << "Compiling fragment shader..." << std::endl;
		glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
		glCompileShader(fragmentShader);

		// Check fragment shader
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> fragmentShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(fragmentShader, logLength, NULL, &fragmentShaderError[0]);
		std::cout << &fragmentShaderError[0] << std::endl;

		// Link program
		std::cout << "Linking program" << std::endl;
		m_ProgramID = glCreateProgram();
		glAttachShader(m_ProgramID, vertexShader);
		glAttachShader(m_ProgramID, fragmentShader);
		glLinkProgram(m_ProgramID);

		// Check program
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &result);
		glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programError((logLength > 1) ? logLength : 1);
		glGetProgramInfoLog(m_ProgramID, logLength, NULL, &programError[0]);
		std::cout << &programError[0] << std::endl;

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::use() {
		glUseProgram(this->m_ProgramID);
	}
}