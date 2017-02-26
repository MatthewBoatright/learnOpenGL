#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <soil.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils\fileUtils.h"
#include "shaders\shader.h"
#include "objects\element_buffer.h"
#include "objects\vertex_array.h"
#include "objects\vertex_buffer.h"
#include "objects\texture.h"

using namespace fileUtilities;
using namespace rendering;

#define WIREFRAME_MODE false

const GLuint WIDTH = 1920, HEIGHT = 1080;

// Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//GLfloat vertices[] = {
//	 // Positions         // Colors           // Texture Coords
//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right		0
//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right		1
//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left		2
//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left			3
//};

GLfloat vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

GLuint indices[] = {  // Note that we start from 0!
	0, 1, 3,		  // First Triangle
	1, 2, 3			  // Second Triangle
};


std::string vertexShaderFilePath = "src/shaders/basicShader.vert";
std::string fragmentShaderFilePath = "src/shaders/basicShader.frag";
std::string woodTextureFilePath = "assets/wood_texture.jpg";
std::string awesomeFaceFilePath = "assets/awesomeface.png";

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

	// Orthographic matrix (frustum view)
	// Paramters: 1) left coordinates 2) right coordiantes 3) top coordinate 4) bottom coordinate 5) near plane 6) far plane
	glm::mat4 ortho_perspective = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, 0.1f, 100.0f);
	// Perspective matrix 
	// Parameters: 1) field of view 2) aspect ratio 3) near plane 4) far plane
	glm::mat4 perspective_proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	
	// Define viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, WIDTH, HEIGHT);

	// Create a shader program
	//GLuint program = LoadShader(vertexShaderFilePath.c_str(), fragmentShaderFilePath.c_str());
	Shader shader(vertexShaderFilePath.c_str(), fragmentShaderFilePath.c_str());

	// Create textures
	Texture texture1(woodTextureFilePath.c_str());
	Texture texture2(awesomeFaceFilePath.c_str());

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
	//ElementBuffer ebo(indices, sizeof(indices));
	// 6. Bind the EBO
	//ebo.bind();
	// 7. Set the vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	// 8 Unbind the VAO
	vao.unbind();

	// Wireframe mode
	if (WIREFRAME_MODE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activated
		glfwPollEvents();

		// Clear the colorbuffer
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

		//GLfloat timeValue = glfwGetTime();
		//GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		//GLint vertexColorLocation = glGetUniformLocation(shader.getProgram(), "someUniform");

		shader.use();
		
		// Activate the first texture level
		glActiveTexture(GL_TEXTURE0);
		texture1.bind();
		glUniform1i(glGetUniformLocation(shader.getProgram(), "ourTexture1"), 0);
		// Activate the second texture level
		glActiveTexture(GL_TEXTURE1);
		texture2.bind();
		glUniform1i(glGetUniformLocation(shader.getProgram(), "ourTexture2"), 1);

		
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		GLint modelLoc = glGetUniformLocation(shader.getProgram(), "model");
		GLint viewLoc = glGetUniformLocation(shader.getProgram(), "view");
		GLint projectionLoc = glGetUniformLocation(shader.getProgram(), "projection");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		vao.bind();
		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		vao.unbind();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

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