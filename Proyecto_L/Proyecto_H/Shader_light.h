#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glew.h>
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
/**
 * @brief Clase para tratar los shader
 * 
 */
class Shader
{
public:
	/**
	 * @brief Construct a new Shader object
	 * 
	 */
	Shader();
	/**
	 * @brief Create a From String object
	 * Crea un shader con el texto del codigo del shader
	 * @param vertexCode Codigo del vertex shader
	 * @param fragmentCode Codigo del fragment shader
	 */
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	/**
	 * @brief Create a From Files object
	 * Crea un shader con un archivo de codigo de shader
	 * @param vertexLocation Archivo de vertex shader
	 * @param fragmentLocation Archivo de fragment shader
	 */
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
	/**
	 * @brief Lee un archivo y retorna su contendio en string
	 * 
	 * @param fileLocation Ruta del archivo
	 * @return std::string 
	 */
	std::string ReadFile(const char* fileLocation);
	/**
	 * @brief Get the Projection Location object 
	 * para la variable uniforme de proyeccion
	 * @return GLuint 
	 */
	GLuint GetProjectionLocation();
	/**
	 * @brief Get the Model Location object
	 * Para la variable uniforme de modelo
	 * @return GLuint 
	 */
	GLuint GetModelLocation();
	/**
	 * @brief Get the View Location object
	 * Para la variable uniforme de vista o view
	 * @return GLuint 
	 */
	GLuint GetViewLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetAmbientcolorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetSpecularIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetEyePositionLocation();
	/**
	 * @brief Set the Directional Light object
	 * Recibe un arreglo de luz direccional y la manda al shader
	 * 
	 * @param dLight 
	 */
	void SetDirectionalLight(DirectionalLight * dLight);
	/**
	 * @brief Set the Point Lights object
	 * Recibe un arreglo de luz puntual y la manda a shader
	 * @param pLight 
	 * @param lightCount 
	 */
	void SetPointLights(PointLight * pLight, unsigned int lightCount);
	/**
	 * @brief Set the Spot Lights object
	 * Recibe un arreglo de spotlight y la manda a shader
	 * @param sLight 
	 * @param lightCount 
	 */
	void SetSpotLights(SpotLight * sLight, unsigned int lightCount);
	/**
	 * @brief Usa el shader para renderizar las variables uniformes
	 * 
	 */
	void UseShader();
	/**
	 * @brief Limpia datos del shader
	 * 
	 */
	void ClearShader();

	~Shader();

private:
	int pointLightCount;
	int spotLightCount;
	GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition,
		uniformSpecularIntensity, uniformShininess;

	struct {
		GLuint uniformcolor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLuint uniformPointLightCount;

	struct {
		GLuint uniformcolor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];


	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

