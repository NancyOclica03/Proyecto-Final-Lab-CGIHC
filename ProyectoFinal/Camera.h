#pragma once // Evita que este archivo se incluya más de una vez en el proyecto

// Librerías necesarias
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Enumeración de movimientos posibles de la cámara
enum Camera_Movement
{
	FORWARD,   // Avanzar
	BACKWARD,  // Retroceder
	LEFT,      // Mover a la izquierda
	RIGHT      // Mover a la derecha
};

// Valores por defecto de la cámara
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

// Clase Camera
class Camera
{
public:
	// Constructor con vectores: permite definir posición, orientación y dirección inicial
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = position;
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor alternativo con valores escalares
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Devuelve la matriz de vista (View Matrix) calculada con LookAt
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->position, this->position + this->front, this->up);
	}

	// Procesa la entrada del teclado y mueve la cámara según la dirección
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->movementSpeed * deltaTime;

		if (direction == FORWARD)
		{
			this->position += this->front * velocity;
		}

		if (direction == BACKWARD)
		{
			this->position -= this->front * velocity;
		}

		if (direction == LEFT)
		{
			this->position -= this->right * velocity;
		}

		if (direction == RIGHT)
		{
			this->position += this->right * velocity;
		}
	}

	// Procesa el movimiento del ratón (rotación de la cámara)
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw += xOffset;
		this->pitch += yOffset;


		if (constrainPitch)
		{
			if (this->pitch > 89.0f)
			{
				this->pitch = 89.0f;
			}

			if (this->pitch < -89.0f)
			{
				this->pitch = -89.0f;
			}
		}

		this->updateCameraVectors();
	}

	// Control del zoom con la rueda del ratón (no implementado)
	void ProcessMouseScroll(GLfloat yOffset)
	{

	}

	// Devuelve el valor actual del zoom (FOV)
	GLfloat GetZoom()
	{
		return this->zoom;
	}

	// Devuelve la posición actual de la cámara
	glm::vec3 GetPosition()
	{
		return this->position;
	}

	// Devuelve el vector frontal
	glm::vec3 GetFront()
	{
		return this->front;
	}

private:
	// Atributos de la cámara
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Ángulos EULER
	GLfloat yaw;
	GLfloat pitch;

	// Opciones de movimiento
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	// Calcula los vectores direccionales (Front, Right, Up) basados en los ángulos actuales
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		
		// Recalcula los vectores "right" y "up" usando producto cruzado
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};