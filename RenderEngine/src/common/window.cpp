#include "window.h"

namespace common {
	Window::Window(unsigned int width, unsigned int height, char* title) : m_width(width), m_height(height), m_title(title) {
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
		m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
		glfwMakeContextCurrent(m_window);

		// Set Callbacks
		// Mouse callback
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		//glfwSetCursorPosCallback(m_window, mouse_callback);
		// Scroll callback
		//glfwSetScrollCallback(m_window, scroll_callback);
		// Key callback
		//glfwSetKeyCallback(m_window, key_callback);

		// Set this to true so GLEW knows to use a modern approach to retrieving function points and extensions
		glewExperimental = GL_TRUE;

		// Init GLEW
		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW." << std::endl;
			//return -1;
		}

		// Define viewport
		int _width, _height;
		glfwGetFramebufferSize(m_window, &_width, &_height);
		glViewport(0, 0, m_width, m_height);

		glEnable(GL_DEPTH_TEST);
	}

	Window::~Window() {}

	void Window::update()  {
		GLfloat currentFrame = glfwGetTime();
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		// Check if any events have been activated
		glfwPollEvents();
		do_movement();
		glfwSwapBuffers(m_window);
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(m_window);
	}

	// Callback function for mouse movements
	void Window::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		static bool firstMouse = true;

		// Mouse position values
		static GLfloat lastX = 1920 / 2.0f;
		static GLfloat lastY = 1080 / 2.0f;
		static GLfloat yaw = -90.0f;
		static GLfloat pitch = 0.0f;

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

		GLfloat sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch -= yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		m_cameraFront = glm::normalize(front);
	}

	// Callback function for scroll wheel
	void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		static GLfloat fov = 45.0f;

		if (fov >= 1.0f && fov <= 45.0f)
			fov -= yoffset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}

	// Callback function for key strokes
	void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		// Application controls
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key >= 0 && key < 1024) {
			if (action == GLFW_PRESS)
				m_keys[key] = true;
			else if (action == GLFW_RELEASE)
				m_keys[key] = false;
		}
	}

	void Window::do_movement()
	{
		// Delta Time


		GLfloat cameraSpeed = 5.0f * m_deltaTime;
		if (m_keys[GLFW_KEY_W])
			m_cameraPos += cameraSpeed * m_cameraFront;
		if (m_keys[GLFW_KEY_A])
			m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
		if (m_keys[GLFW_KEY_S])
			m_cameraPos -= cameraSpeed * m_cameraFront;
		if (m_keys[GLFW_KEY_D])
			m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
	}
}