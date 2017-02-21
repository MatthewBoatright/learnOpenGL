#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#define WIREFRAME_MODE false

const GLuint WIDTH = 1920, HEIGHT = 1080;

// Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
std::string ReadFile(const char* filePath);
GLuint LoadShader(const char* vertexPath, const char* fragmentPath);

GLfloat vertices[] = 
{
	 0.5f,  0.5f, 0.0f, 
	 0.5f, -0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f 
};

GLuint indices[] = 
{
	0, 1, 3,   
	1, 2, 3    
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

	// Generate buffers
	GLuint VBO;
	glGenBuffers(1, &VBO);
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint EBO;
	glGenBuffers(1, &EBO);

	// 1.) Bind VAO
	glBindVertexArray(VAO);
	// 2.) Copy vertices array into buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.) Copy index data into buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 4.) Set vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 5.) Unbind the VAO
	glBindVertexArray(0);

	// Wireframe mode
	if (WIREFRAME_MODE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// De-allocate used resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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

// Return the contents of a file
std::string ReadFile(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

// Return the ID of a loaded shader
GLuint LoadShader(const char* vertexPath, const char* fragmentPath)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read Shaders
	std::string vertexShaderStr = ReadFile(vertexPath);
	std::string fragmentShaderStr = ReadFile(fragmentPath);
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