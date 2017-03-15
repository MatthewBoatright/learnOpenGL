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

#include "common\window.h"
#include "common\camera.h"
#include "utils\fileUtils.h"
#include "shaders\shader.h"
#include "objects\element_buffer.h"
#include "objects\vertex_array.h"
#include "objects\vertex_buffer.h"
#include "objects\texture.h"

using namespace fileUtilities;
using namespace rendering;
using namespace common;

const GLuint WIDTH = 1920, HEIGHT = 1080;

// Prototypes
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void basicOpenGL();
void lightingOpenGL();

// Camera vectors
glm::vec3 cameraPos		= glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront	= glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp		= glm::vec3(0.0f, 1.0f, 0.0f);

// Delta Time
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;	// Time of last frame

// Key callback array
bool keys[1024];

// Mouse position values
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

GLfloat fov = 45.0f;

Window* window;
Camera* camera;

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	window = new Window(WIDTH, HEIGHT, "Basic Rendering Engine");
	// Mouse callback
	glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
	// Scroll callback
	glfwSetScrollCallback(window->getWindow(), scroll_callback);
	// Key callback
	glfwSetKeyCallback(window->getWindow(), key_callback);

	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	// Select demonstration
	//basicOpenGL();
	lightingOpenGL();

	// Terminate GLFW
	glfwTerminate();

	return 0;
}

void lightingOpenGL() {
	// Assets
	GLfloat vertices[] = {
	    -0.5f, -0.5f, -0.5f,
	     0.5f, -0.5f, -0.5f,
	     0.5f,  0.5f, -0.5f,
	     0.5f,  0.5f, -0.5f,
	    -0.5f,  0.5f, -0.5f,
	    -0.5f, -0.5f, -0.5f,
	
	    -0.5f, -0.5f,  0.5f,
	     0.5f, -0.5f,  0.5f,
	     0.5f,  0.5f,  0.5f,
	     0.5f,  0.5f,  0.5f,
	    -0.5f,  0.5f,  0.5f,
	    -0.5f, -0.5f,  0.5f,
	
	    -0.5f,  0.5f,  0.5f,
	    -0.5f,  0.5f, -0.5f,
	    -0.5f, -0.5f, -0.5f,
	    -0.5f, -0.5f, -0.5f,
	    -0.5f, -0.5f,  0.5f,
	    -0.5f,  0.5f,  0.5f,
	
	     0.5f,  0.5f,  0.5f,
	     0.5f,  0.5f, -0.5f,
	     0.5f, -0.5f, -0.5f,
	     0.5f, -0.5f, -0.5f,
	     0.5f, -0.5f,  0.5f,
	     0.5f,  0.5f,  0.5f,
	
	    -0.5f, -0.5f, -0.5f,
	     0.5f, -0.5f, -0.5f,
	     0.5f, -0.5f,  0.5f,
	     0.5f, -0.5f,  0.5f,
	    -0.5f, -0.5f,  0.5f,
	    -0.5f, -0.5f, -0.5f,
	
	    -0.5f,  0.5f, -0.5f,
	     0.5f,  0.5f, -0.5f,
	     0.5f,  0.5f,  0.5f,
	     0.5f,  0.5f,  0.5f,
	    -0.5f,  0.5f,  0.5f,
	    -0.5f,  0.5f, -0.5f
    };

	// Light attributes
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	Shader lampShader("src/shaders/lampShader.vert", "src/shaders/lampShader.frag");
	Shader lightingShader("src/shaders/lightingShader.vert", "src/shaders/lightingShader.frag");

	VertexArray containerVAO;
	VertexBuffer vbo(vertices, sizeof(vertices));

	vbo.bind();
	containerVAO.bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);	
	containerVAO.unbind();

	VertexArray lightVAO;

	lightVAO.bind();
	vbo.bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	lightVAO.unbind();

	while (!window->shouldClose()) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activated
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightingShader.use();
		GLint objectColorLoc = glGetUniformLocation(lightingShader.getProgram(), "objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.getProgram(), "lightColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);

		// MVP
		glm::mat4 model;
		glm::mat4 projection;
		glm::mat4 view;
		// View matrix
		view = camera->getViewMatrix();
		// Projeciton matrix
		projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Get location ID
		GLint modelLoc = glGetUniformLocation(lightingShader.getProgram(), "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.getProgram(), "view");
		GLint projectionLoc = glGetUniformLocation(lightingShader.getProgram(), "projection");

		// Set shader uniforms to the glm matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		containerVAO.bind();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		containerVAO.unbind();

		lampShader.use();

		modelLoc = glGetUniformLocation(lampShader.getProgram(), "model");
		viewLoc = glGetUniformLocation(lampShader.getProgram(), "view");
		projectionLoc = glGetUniformLocation(lampShader.getProgram(), "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightVAO.bind();
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightVAO.unbind();		

		// Swap the screen buffers
		window->update();
	}
}

void basicOpenGL() {
	// Assets
	GLfloat vertices[] = 
	{
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

	glm::vec3 cubePositions[] = 
	{
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	GLuint indices[] = 
	{					  // Note that we start from 0!
		0, 1, 3,		  // First Triangle
		1, 2, 3			  // Second Triangle
	};

	std::string woodTextureFilePath = "assets/wood_texture.jpg";
	std::string awesomeFaceFilePath = "assets/awesomeface.png";

	// Create a shader program
	Shader shader("src/shaders/basicShader.vert", "src/shaders/basicShader.frag");

	// Create textures
	Texture texture1(woodTextureFilePath.c_str());
	Texture texture2(awesomeFaceFilePath.c_str());

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

	// Render loop
	while (!window->shouldClose())
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activated
		glfwPollEvents();
		do_movement();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		shader.use();

		// Activate the first texture level
		glActiveTexture(GL_TEXTURE0);
		texture1.bind();
		glUniform1i(glGetUniformLocation(shader.getProgram(), "ourTexture1"), 0);

		// Activate the second texture level
		glActiveTexture(GL_TEXTURE1);
		texture2.bind();
		glUniform1i(glGetUniformLocation(shader.getProgram(), "ourTexture2"), 1);

		// MVP
		glm::mat4 model;
		glm::mat4 projection;
		glm::mat4 view;

		// View matrix
		view = camera->getViewMatrix();

		// Projeciton matrix
		projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Get location ID
		GLint modelLoc = glGetUniformLocation(shader.getProgram(), "model");
		GLint viewLoc = glGetUniformLocation(shader.getProgram(), "view");
		GLint projectionLoc = glGetUniformLocation(shader.getProgram(), "projection");

		// Set shader uniforms to the glm matrices
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
		vao.unbind();

		// Swap the screen buffers
		window->update();
	}
}

// Callback function for mouse movements
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	camera->processMouseMovement(xoffset, yoffset);
}

// Callback function for scroll wheel
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->proccessMouseScroll(yoffset);
}

// Callback function for key strokes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	static bool wireFrameMode = false;

	// Application controls
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Wireframe mode
	if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS)
	{
		// Flip wireframe mode
		wireFrameMode = !wireFrameMode;

		if (wireFrameMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement() 
{
	if (keys[GLFW_KEY_W])
		camera->processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera->processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_S])
		camera->processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_D])
		camera->processKeyboard(RIGHT, deltaTime);
}