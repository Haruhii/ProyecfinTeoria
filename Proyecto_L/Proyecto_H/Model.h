#pragma once
#include <vector>
#include<string>
#include<stdio.h>
#include<time.h>
#include<assimp/Importer.hpp>
#include<assimp/Exporter.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include <filesystem>
#include "Mesh.h"
#include "Texture.h"
#ifdef __unix__
	namespace fs = std::filesystem;
#elif _WIN64 || _WIN32
	namespace fs = std::experimental::filesystem;
#endif

/**
 * @brief Clase Model 
 * 
 */
class Model
{
public:
	/**
	 * @brief Construct a new Model object
	 * 
	 */
	Model();
	/**
	 * @brief Carga un modelo a el mesh
	 * 
	 * @param fileName Archivo de modelo .obj o .assbin o .3ds o compatible
	 */
	void LoadModel(const std::string& fileName);
	/**
	 * @brief Carga un modelo y lo convierte a .assbin
	 * El formato .assbin tiene un tiempo de carga 100% superior al .obj
	 * @param fileName Ruta del archivo compatible
	 * @param fileNameOut Ruta del archivo assbin 
	 */
	void LoadModel(const std::string & fileName, const std::string & fileNameOut);
	/**
	 * @brief Renderiza el mesh del model
	 * 
	 */
	void RenderModel();
	/**
	 * @brief Limpia los datos del modelo
	 * 
	 */
	void ClearModel();
	/**
	 * @brief Destroy the Model object
	 * Destruyer
	 */
	~Model();

private:
	/**
	 * @brief Carga un nodo a la escena
	 * 
	 * @param node 
	 * @param scene 
	 */
	void LoadNode(aiNode* node, const aiScene* scene); //assimp
	/**
	 * @brief Carga una malla a la escena
	 * 
	 * @param mesh 
	 * @param scene 
	 */
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	/**
	 * @brief Carga los materiales que consisten en texturas en archivos .mtl
	 * Por defecto carga la textura plain
	 * @param scene 
	 */
	void LoadMaterials(const aiScene* scene);
	const aiScene *scene;
	std::vector<Mesh*>MeshList;
	std::vector<Texture*>TextureList;
	std::vector<unsigned int>meshTotex;
};

