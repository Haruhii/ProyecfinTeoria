#pragma once

#include <glew.h>
#include <glm.hpp>

/**
 * @brief Clase base de la iluminacion
 * 
 */
class Light
{
public:
	Light();
	/**
	 * @brief Constructor de la clase Light
	 * 
	 * @param red Intensidad de color rojo [0,1]
	 * @param green Intensidad de color verde [0,1]
	 * @param blue Intensidad de color azul [0,1]
	 * @param aIntensity Intensidad ambiental
	 * @param dIntensity Intensidad difusa
	 */
	Light(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);
	/**
	 * @brief Pone la intensidad del la luz
	 * 
	 * @param aIntensity Intensidad ambiental
	 * @param dIntensity Intensidad difusa
	 */
	void setIntensity(GLfloat aIntensity, GLfloat dIntensity);
	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

