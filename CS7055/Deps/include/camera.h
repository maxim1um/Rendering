#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

enum Camera_Movement{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

GLfloat SPEED = 2.5f;
GLfloat SENSITIVITY = 0.002f;
GLfloat ZOOM = 45.0f;

class Camera
{
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::vec3 startFront;

	// quaternion
	glm::quat Quaternion;
	glm::quat lastQuaternion;
	// camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		startFront = Front;
		Quaternion = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		updateCameraVectors();
	}

	// constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		startFront = Front;
		Quaternion = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
		updateCameraVectors();
	}

	// returns the view matrix calculated using euler angles and the lookat matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyborad(Camera_Movement direcction, GLfloat deltaTime)
	{
		GLfloat velocity = MovementSpeed * deltaTime;
		if (direcction == FORWARD)
		{
			Position += Front * velocity;
		}
		if (direcction == BACKWARD)
		{
			Position -= Front * velocity;
		}
		if (direcction == LEFT)
		{
			Position -= Right * velocity;
		}
		if (direcction == RIGHT)
		{
			Position += Right * velocity;
		}
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		glm::quat changes = glm::quat(glm::vec3(yoffset, xoffset, 0.0f));
		lastQuaternion = Quaternion;
		Quaternion = glm::cross(changes, Quaternion);
		Quaternion = glm::normalize(Quaternion);

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
		{
			Zoom -= yoffset;
		}
		if (Zoom <= 1.0f)
		{
			Zoom = 1.0f;
		}
		if (Zoom >= 45.0f)
		{
			Zoom = 45.0f;
		}
	}

private:
	// calculates the front vector from the camera's eluer angles
	void updateCameraVectors()
	{

		// re-calculate the right and up vector
		glm::quat slurp = glm::slerp(lastQuaternion, Quaternion, 0.5f);
		glm::mat3 rotationMatrix = glm::mat3_cast(slurp);
		Front = startFront * rotationMatrix;
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif
