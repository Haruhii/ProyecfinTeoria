#pragma once
#include<glew.h>
/**
 * @brief Clase textura
 * 
 */
class Texture
{
public:
	/**
	 * @brief Construct a new Texture object
	 * 
	 */
	Texture();
	/**
	 * @brief Construct a new Texture object
	 * Constructor de textura. Recibe el archivo de la textura a dibujar
	 * @param FileLoc 
	 */
	Texture(const char* FileLoc);
	/**
	 * @brief Textura sin canal alfa
	 * 
	 * @return true 
	 * @return false 
	 */
	bool LoadTexture();
	/**
	 * @brief Textura con canal alfa
	 * 
	 * @return true 
	 * @return false 
	 */
	bool LoadTextureA();
	/**
	 * @brief Manda los datos al shader
	 * 
	 */
	void UseTexture();
	/**
	 * @brief Limpia datos de la textura
	 * 
	 */
	void ClearTexture();
	/**
	 * @brief Destroy the Texture object
	 * 
	 */
	~Texture();
private: 
	GLuint textureID;
	int width, height, bitDepth;
	const char *fileLocation;

};

