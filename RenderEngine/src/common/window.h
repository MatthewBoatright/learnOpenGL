#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

namespace common {
	class Window {
	public:
		Window(unsigned int width, unsigned int height, char* title);
		~Window();	
		void update();
		bool shouldClose() const;

		// Temp func
		inline GLFWwindow* getWindow() const { return m_window; }
	private:
		char* m_title;
		GLuint m_width;
		GLuint m_height;
		GLFWwindow* m_window;

		// Callback functions
		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
		void do_movement();

		// Camera vectors
		glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		GLfloat m_deltaTime = 0.0f;	// Time between current frame and last frame
		GLfloat m_lastFrame = 0.0f;	// Time of last frame

		bool m_keys[1024];
	};
}

#endif