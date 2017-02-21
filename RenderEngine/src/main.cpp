#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "utils\fileUtils.h"
#include "objects\element_buffer.h"
#include "objects\vertex_array.h"
#include "objects\vertex_buffer.h"

using namespace fileUtilities;
using namespace rendering;

#define WIREFRAME_MODE false

const GLuint WIDTH = 1920, HEIGHT = 1080;

// Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint LoadShader(const char* vertexPath, const char* fragmentPath);

GLfloat vertices[] = {
	 // Positions         // Colors        
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // Top Right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // Bottom Right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, // Bottom Left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f  // Top Left 
};
GLuint indices[] = {  // Note that we start from 0!
	0, 1, 3,		  // First Triangle
	1, 2, 3			  // Second Triangle
};

std::string vertexShaderFilePath = "src/shaders/basicShader.vert";
std::string fragmentShaderFilePath = "src/shaders/basicShader.frag";

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	// Init GLFW
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW." << std::endl;
	}

	// Set GLFW options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Basic Rendering Project", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set Callbacks
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function points and extensions
	glewExperimental = GL_TRUE;
	
	// Init GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW." << std::endl;
		return -1;
	}
	// Define viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Create a shader program
	GLuint program = LoadShader(vertexShaderFilePath.c_str(), fragmentShaderFilePath.c_str());

	/* Without wrapper classes */
	//GLuint VBO, VAO, EBO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	//// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	//glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	/* With wrapper classes */
	// 1. Create a VAO
	VertexArray vao;
	// 2. Bind the VAO
	vao.bind();
	// 3. Load the VAO with a new VBO
	VertexBuffer vbo(vertices, sizeof(vertices));
	// 4. Bind the VBO
	vbo.bind();
	// 5. Load the VAO with a new EBO
	ElementBuffer ebo(indices, sizeof(indices));
	// 6. Bind the EBO
	ebo.bind();
	// 7. Set the vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// 8 Unbind the VAO
	vao.unbind();

	// Wireframe mode
	if (WIREFRAME_MODE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Get the max vertex attributes for current hardware
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "MAX VERTEX ATTRIBS: " << nrAttributes << std::endl;

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		glUseProgram(program);
		vao.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		vao.unbind();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// De-allocate used resources
	//glDeleteVertexArrays(1, &VAO);
	
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	// Terminate GLFW
	glfwTerminate();

	return 0;
}

// Callback function for key strokes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

// Return the ID of a loaded shader
GLuint LoadShader(const char* vertexPath, const char* fragmentPath)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read Shaders
	std::string vertexShaderStr = getFileContents(vertexPath);
	std::string fragmentShaderStr = getFileContents(fragmentPath);
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
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check program
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	std::cout << &programError[0] << std::endl;

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}