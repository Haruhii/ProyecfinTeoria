#pragma once

#include <glew.h>
/**
 * @brief Esta clase está pensada para crear materiales asignables a nuestros diferentes objetos y luces
 * 
 */
class Material
{
public:
	/**
	 * @brief Constructor de un objeto Material
	 * 
	 */
	Material();
	/**
	 * @brief Constructor de un objeto Material
	 * 
	 * @param sIntensity Intensidad especular
	 * @param shine Brillo
	 */
	Material(GLfloat sIntensity, GLfloat shine);
	/**
	 * @brief Manda material a variable uniforme para el shader
	 * 
	 * @param specularIntensityLocation Indice de la variable uniforme
	 * @param shininessLocation Indice de la variable uniforme
	 */
	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
	/**
	 * @brief Destructor
	 * 
	 */
	~Material();

private: 
	GLfloat specularIntensity;
	GLfloat shininess;
};

