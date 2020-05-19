#pragma once
#include "PointLight.h"
/**
 * @brief Clase SpotLight
 * Clase para luz de tipo Spot, hereda de PointLight
 */
class SpotLight :
	public PointLight
{
public:
	/**
	 * @brief Construct a new Spot Light object
	 * 
	 */
	SpotLight();
	/**
	 * @brief Construct a new Spot Light object
	 * Recibe colores, intensidad ambiental, difusa, posicion, color, direccion y atenuacion
	 * @param red 
	 * @param green 
	 * @param blue 
	 * @param aIntensity 
	 * @param dIntensity 
	 * @param xPos 
	 * @param yPos 
	 * @param zPos 
	 * @param xDir 
	 * @param yDir 
	 * @param zDir 
	 * @param con 
	 * @param lin 
	 * @param exp 
	 * @param edg 
	 */
	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp,
		GLfloat edg);
	/**
	 * @brief Usar la luz, manda los datos de las variables uniformes a el shader
	 * 
	 * @param ambientIntensityLocation 
	 * @param ambientColourLocation 
	 * @param diffuseIntensityLocation 
	 * @param positionLocation 
	 * @param directionLocation 
	 * @param constantLocation 
	 * @param linearLocation 
	 * @param exponentLocation 
	 * @param edgeLocation 
	 */
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
		GLuint edgeLocation);
	/**
	 * @brief Set the Flash object
	 * 
	 * @param pos 
	 * @param dir 
	 */
	void SetFlash(glm::vec3 pos, glm::vec3 dir);
	/**
	 * @brief Set the Color object
	 * 
	 * @param col 
	 */
	void SetColor(glm::vec3 col);
	void SetDirection(glm::vec3 dir);
	~SpotLight();

private:
	glm::vec3 direction;

	GLfloat edge, procEdge;
};

