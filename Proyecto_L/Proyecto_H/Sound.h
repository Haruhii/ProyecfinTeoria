#pragma once
#include <irrKlang.h>
#include <stdio.h>
using namespace irrklang;
/**
 * @brief Clase de sonido que sirve para crear efectos de sonido
 * 
 */
class Sound
{
public:
	/**
	 * @brief Construct a new Sound object
	 * 
	 */
	Sound();
	/**
	 * @brief Construct a new Sound object
	 * Recibe la posicion con respecto del espectador para crear el sonido. Por default es 0,0,0
	 * @param posx 
	 * @param posy 
	 * @param posz 
	 */
	Sound(float posx,float posy,float posz);
	/**
	 * @brief Reproduce musica ambiental en un loop infinito
	 * 
	 * @param path Ruta del archivo de audio a reproducir
	 */
	void playMusic(const char* path);
	/**
	 * @brief Reproduce un efecto de sonido
	 * 
	 * @param file Ruta del archivo de audio
	 */
	void playFX(const char* file);
	/**
	 * @brief Destroy the Sound object
	 * 
	 */
	~Sound();
private: 
	/**
	 * @brief Motor de reproduccion de audio
	 * 
	 */
	ISoundEngine* engine = createIrrKlangDevice();
	/**
	 * @brief Instancia de reproduccion de audio
	 * 
	 */
	ISound* music;
    float pos_x;
    float pos_y;
    float pos_z;
};
