#pragma once

#include <glew.h>
/**
 * @brief Clase Mesh que corresponde a la malla 
 * 
 */
class Mesh
{
public:
	/**
	 * @brief Construct a new Mesh object
	 * 
	 */
	Mesh();
	/**
	 * @brief Crea un objeto mesh
	 * 
	 * @param vertices arreglo de vertices
	 * @param indices arreglo de indices
	 * @param numOfVertices Numero de vertices
	 * @param numOfIndices Numero de indices
	 */
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	/**
	 * @brief Create una malla con coordenadas de textura y normales
	 * 
	 * @param vertices 
	 * @param textcoords Arreglo de coordenadas de texturas
	 * @param normals Arreglo de normales
	 * @param indices 
	 * @param numOfVertices 
	 * @param numOfTexcoords 
	 * @param numOfNormals 
	 * @param numOfIndices 
	 */
	void CreateMesh(GLfloat *vertices, GLfloat *textcoords, GLfloat *normals, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfTexcoords, unsigned int numOfNormals, unsigned int numOfIndices);
	/**
	 * @brief Renderiza la malla
	 * 
	 */
	void RenderMesh();
	/**
	 * @brief Limpia los datos de la malla
	 * 
	 */
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

