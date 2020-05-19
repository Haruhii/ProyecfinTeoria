#pragma once
#include "Light.h"
/**
 * @brief Clase PointLight para luz puntual
 * 
 */
class PointLight :
	public Light
{
public:
	/**
	 * @brief Construct a new Point Light object
	 * 
	 */
	PointLight();
	/**
	 * @brief Construct a new Point Light object
	 * Construye una nueva luz puntual, recibe color intensidad y atenuacion
	 * 
	 * @param red Color rojo
	 * @param green Color verde
	 * @param blue Color azul
	 * @param aIntensity Intensidad ambiental
	 * @param dIntensity Intensidad difusa
	 * @param xPos Posicion en X
	 * @param yPos Posicion en Y
	 * @param zPos Posicion en Z
	 * @param con Constante
	 * @param lin Lineal
	 * @param exp Exponencial
	 */
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);
	/**
	 * @brief Usa la luz en el shader pasandole las variables uniferokmes
	 * 
	 * @param ambientIntensityLocation 
	 * @param ambientcolorLocation 
	 * @param diffuseIntensityLocation 
	 * @param positionLocation 
	 * @param constantLocation 
	 * @param linearLocation 
	 * @param exponentLocation 
	 */
	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientcolorLocation,
		GLfloat diffuseIntensityLocation, GLfloat positionLocation,
		GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);
	/**
	 * @brief Set the Position de la luz
	 * 
	 * @param pos_x 
	 * @param pos_y 
	 * @param pos_z 
	 */
	void SetPosition(GLfloat pos_x, GLfloat pos_y, GLfloat pos_z);
	/**
	 * @brief Set the Color
	 * 
	 * @param red 
	 * @param green 
	 * @param blue 
	 */
	void SetColor(GLfloat red, GLfloat green, GLfloat blue);
	~PointLight();

protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};

