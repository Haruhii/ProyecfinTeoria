#pragma once

#include "CommonValues.h"
#include <vector>
#include <string>
#include<glew.h>
#include<glm.hpp>
#include<glfw3.h>
#include <gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include "Mesh.h"
#include "Shader_light.h"
/**
 * @brief Clase skybox para dibujar un skybox en el viewport
 * 
 */
class Skybox
{
public:
	/**
	 * @brief Construct a new Skybox object
	 * 
	 */
	Skybox();
	/**
	 * @brief Construct a new Skybox object
	 * Recive un vector con las localizaciones de las texturas del skybox
	 * @param faceLocations Vector de rutas de archivos
	 */
	Skybox(std::vector<std::string> faceLocations);
	/**
	 * @brief Dibuja el skybox con su shader correspondiente
	 * 
	 * @param viewMatrix matriz de vista
	 * @param projectionMatrix matriz de proyeccion
	 */
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	/**
	 * @brief Destroy the Skybox object
	 * 
	 */
	~Skybox();
private:
	Mesh* skyMesh;
	Shader* skyShader;
	GLuint textureId;
	GLuint uniformProjection, uniformView;

};

