#pragma once
#include<stdio.h>
#include <glfw3.h>
#include <stdlib.h>
#include <string.h>
/**
 * @brief Estructura de las variables modificables en los keyframes
 * 
 */
typedef struct _frame
{
	float mov_x = 0.0f;
	float mov_y = 0.0f;
	float mov_z = 0.0f;
	float mov_xInc = 0.0f;
	float mov_yInc = 0.0f;
	float mov_zInc = 0.0f;
	float giro_x = 0.0f;
	float giro_y = 0.0f;
	float giro_z = 0.0f;
	float giro_xinc = 0.0f;
	float giro_yinc = 0.0f;
	float giro_zinc = 0.0f;
	float scale_x = 0.0f;
	float scale_y = 0.0f;
	float scale_z = 0.0f;
	float scale_xinc = 0.0f;
	float scale_yinc = 0.0f;
	float scale_zinc = 0.0f;
}FRAME;
/**
 *  Clase Keyframe. Se encarga de parsear y poner en ejecucion las animacion keyframe
 */
class Keyframe
{
public:
	/*
	* Constructor por default.
	*/
	Keyframe();
	/**
	* @brief  Constructor que recibe parametros
	* @param keyFile Es el nombre del archivo que contiene los keyframe a parsear
	* @param _numFrames Recibe el numero de frames maximo que soporta el parser
	* @param _id_key Recibe el id o nombre del keyframe. No tiene utilidad alguna mas que para debug
	*/
	Keyframe(const char* keyFile, int _numFrames, const char* _id_key);
	/**
	 * @brief Parser del archivo de texto
	 * Lee el archivo de texto y llena la estructura para cargar los keyframes de la animacion.
	 * 
	 */
	void parse_key_frames();
	/**
	 * @brief Itera la animacion
	 * Este corre la secuencia de keyframes para devolver los valores de animacion.
	 */
	void animate();
	/**
	 * @brief Interplacion de movimiento
	 * Genera los movimientos intermedios de transicion de todos los keyframes
	 */
	void interpolation();
	/**
	 * @brief calcula una interpolcion de un valor en especifico
	 * Interpola segun los parametros dados
	 * @param inc Valor de incremento del keyframe dado
	 * @param nVal Valor siguiente del keyframe
	 * @param val Valor actual de la animacion
	 */
	void interpolate(float *inc, float *nVal, float *val);
	/**
	 * @brief Retorna los valores de keyframes a su valor por default
	 * 
	 */
	void resetElements();
	/**
	 * @brief setter de la bandera de play
	 * 
	 * @param status 
	 */
	void setPlay(bool status) { play = status; }
	bool getPlay() { return play; }
	/**
	 * @brief Funcion que inicia la animacion segun un evento del exterior
	 * 
	 * 
	 * @param accion Evento del exterior, puede ser valor de una tecla o una bandera de arranque
	 */
	void inputKeyframes(bool accion);
	/**
	 * @brief Obtiene un valor del keyframe
	 * 
	 * @param val El nombre estandar del valor que se desea obtener
	 * @return float Retorna el valor deseado en caso de que sea correcto
	 */
	float getVal(const char* val);
	/**
	 * @brief Destructor
	 * 
	 */
	~Keyframe();
private:
	FILE *kFile;
	const char* nameFile;
	const char* id_keyFrames;
	float mov_x, mov_y, mov_z, giro_x, giro_y, giro_z, scale_x, scale_y, scale_z;
	int i_max_steps = 90;
	int i_curr_steps = 5;
	int numFrames;
	FRAME KeyFrame[100];
	int FrameIndex;
	int reproduciranimacion = 0, habilitaranimacion = 0;
	bool play = false;
	int playIndex = 0;

};