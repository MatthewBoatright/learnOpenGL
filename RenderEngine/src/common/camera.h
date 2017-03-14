#ifndef CAMERA_H
#define CAMERA_H

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

namespace common {

	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat SPEED = 3.0f;
	const GLfloat SENSITIVITY = 0.15f;
	const GLfloat ZOOM = 45.0f;

	class Camera {
	public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		GLfloat Yaw;
		GLfloat Pitch;

		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Zoom;

		Camera
		(	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
			GLfloat yaw = YAW, GLfloat pitch = PITCH
		);

		Camera
		(	GLfloat posX, GLfloat posY, GLfloat posZ, 
			GLfloat upX, GLfloat upY, GLfloat upZ, 
			GLfloat yaw, GLfloat pitch
		);

		glm::mat4 getViewMatrix();
		void processKeyboard(Camera_Movement direction, GLfloat deltaTime);
		void processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
		void proccessMouseScroll(GLfloat yoffset);

	private:

		void updateCameraVectors();
	};
}

#endif