#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <glfw3.h>
/**
 * @brief Clase camara
 * 
 */
class Camera
{
public:
	/**
	 * @brief Construye un nuevo objeto de camara
	 * 
	 */
	Camera();
	/**
	 * @brief Construye un camara recibiendo parametros
	 * 
	 * @param startPosition Posicion de inicio
	 * @param startUp Up
	 * @param startYaw Yaw
	 * @param startPitch Pitch
	 * @param startMoveSpeed Velocidad inicial de movimiento
	 * @param startTurnSpeed Velocidad inicial de rotacion
	 */
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	/**
	 * @brief Control 
	 * 
	 * @param keys 
	 * @param deltaTime 
	 */
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);
	void mouseControlAerea();
	void mouseControlQuiosco();
	void keyControlAerea(bool* keys, GLfloat deltaTime);
	void keyControlAvatar(bool* keys, GLfloat deltaTime);
	void keyControlQuiosco(bool* keys, GLfloat deltaTime);
	glm::vec3 getCameraPosition();
	void setCameraPosition(glm::vec3 pos) { position = pos; };
	glm::vec3 getCameraDirection();
	glm::mat4 calculateViewMatrix();
	

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	GLfloat max_x, min_x, max_y, min_y, max_z, min_z;
	GLfloat yaw;
	GLfloat pitch;

	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

