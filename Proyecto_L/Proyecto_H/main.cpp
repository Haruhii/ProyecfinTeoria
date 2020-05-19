/*
Semestre 2020-2
Proyecto final
Integrantes:
Suárez Hernández Sara Amalinali
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <thread>
#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"
#include "Sound.h"
#include "Keyframe.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
Sound *music = new Sound(0.0f, 0.0f, 0.0f);

Skybox skybox;
Skybox skybox_noche;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLint dia_flag = 0;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0,
uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;

// Modelos
Model *base;
Model *CuerpoPajaro;
Model *AlaDer;
Model *AlaIzq;
Model *Arbol;
Model *Cabeza_avatar;
Model *Cuerpo_avatar;
Model *PiernaD_avatar;
Model *PiernaI_avatar;
Model *BrazoI_avatar;
Model *BrazoD_avatar;
Model Faro;
Model Banca;
Model Fuente;
Model Helipuerto;
Model Helicoptero;
Model Helice_lateral;
Model Helice;


Model *car;
Model *nave;
Model *Depa2;
Model *picina;
Model *pelota;

Model *juego;
Model *banca;
Model *pasamanos;



//Model *base;
// Keyframes
Keyframe *keyframes_pajaro = new Keyframe("Keyframes/keyFramesPajaro.txt", 60, "Pajaro");
Keyframe *keyframes_helicoptero = new Keyframe("Keyframes/keyFramesHelicoptero.txt", 60, "Helicoptero");

// Variables de animacion

glm::vec3 posAvatar(24.0f, 2.4f, 10.0f);

GLfloat giro_offset = 45.0f;
GLfloat rot_helice = 0.0f;
GLfloat mov_alas = 0.0f;
GLfloat movOffset = 1.0f;
GLfloat pos_x_nave = 0.0f;
GLfloat pos_y_nave = 0.0f;
GLfloat pos_z_helicopter = 0.0f;
GLfloat posSpotX[3] = { 0.0f,0.0f,0.0f };
GLfloat posSpotY[3] = { 0.0f,0.0f,0.0f };
GLfloat posSpotZ[3] = { 0.0f,0.0f,0.0f };
GLfloat func_nave = 0.0f;
GLfloat func_sin = 0.0f;
GLfloat rot_y_nave = 0.0f;

GLfloat avatar_rot = 0.0f;
GLfloat rot_brazo = 0.0f;
GLfloat rot_pierna = 0.0f;

GLfloat pos_x_pajaro = 0.0f,
	pos_y_pajaro = 0.0f,
	pos_z_pajaro = 0.0f;

GLint flag_helicoptero = 1;
GLint ciclos = 0;


bool *keys_avatar;
bool alas_sentido = false;
bool brazo_sentido = false;
bool pierna_sentido = false;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = (unsigned int)i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		1.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}
/**
 * @brief dibuja modelos de acuerdo a sus posiciones
 *
 * @param objeto Modelo a repetir
 * @param posiciones Lista de posiciones
 * @param model Matriz de modelo
 * @param uniformModel
 * @param uniformSpecularIntensity
 * @param uniformShininess
 * @param num_posiciones Numero de modelos a dibujar
 */
void loadModelArray(Model objeto, GLfloat* posiciones, glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess, int num_posiciones) {
	glm::vec3 posicion;
	int i_aux = 0;
	posicion = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_posiciones; i++) {
		if (i_aux > 2)
			i_aux = 0;
		if (i_aux == 0) {
			posicion.x = posiciones[i];
		}
		if (i_aux == 1) {
			posicion.y = posiciones[i];
		}
		if (i_aux == 2) {
			posicion.z = posiciones[i];
			model = glm::mat4(1.0);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::translate(model, posicion);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			objeto.RenderModel();
			posicion = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		i_aux++;

	}
}
/**
 * @brief Dibuja objetos con luz puntal
 *
 * @param objeto Modelo a dibujar
 * @param posiciones Lista con posiciones
 * @param model Matriz de modelo
 * @param uniformModel
 * @param uniformSpecularIntensity
 * @param uniformShininess
 * @param ind_point_lights lista con los indices de las luces puntuales para usar
 * @param num_posiciones Numero de faros a dibujare
 */
void loadModelArrayFaro(Model objeto, GLfloat* posiciones, glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess, GLint* ind_point_lights, int num_posiciones) {
	glm::vec3 posicion;
	int i_aux = 0;
	int iL = 0;
	int iL2 = 0;
	
	posicion = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_posiciones; i++) {
		if (i_aux > 2)
			i_aux = 0;
		if (i_aux == 0) {
			posicion.x = posiciones[i];
		}
		if (i_aux == 1) {
			posicion.y = posiciones[i];
		}
		if (i_aux == 2) {
			iL = ind_point_lights[iL2];
			posicion.z = posiciones[i];
			model = glm::mat4(1.0);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			model = glm::translate(model, posicion);
			pointLights[iL].SetPosition(posicion.x, 5.8f, posicion.z);
			if (dia_flag == 0)
				pointLights[iL].SetColor(1.0f, 1.0f, 1.0f);
			else
				pointLights[iL].SetColor(0.0f, 0.0f, 0.0f);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			objeto.RenderModel();
			posicion = glm::vec3(0.0f, 0.0f, 0.0f);
			iL2++;
		}
		i_aux++;
	}
}
/**
 * @brief Carga los arbustos con canal alfa
 *
 * @param mesh Malla, debe ser un plano aunque puede ser cualquier tipo de malla
 * @param posiciones Lista con las posiciones
 * @param model Modelo
 * @param uniformModel Variable unifromde de modelo
 * @param uniformSpecularIntensity
 * @param uniformShininess
 * @param num_posiciones Numero de objetos a dibujar
 */
void loadModelArbustoArray(Mesh* mesh, GLfloat* posiciones, glm::mat4 model, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess, int num_posiciones) {
	glm::vec3 posicion;
	int i_aux = 0;
	posicion = glm::vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_posiciones; i++) {
		if (i_aux > 2)
			i_aux = 0;
		if (i_aux == 0) {
			posicion.x = posiciones[i];
		}
		if (i_aux == 1) {
			posicion.y = posiciones[i];
		}
		if (i_aux == 2) {
			posicion.z = posiciones[i];
			model = glm::mat4(1.0);
			model = glm::translate(model, posicion);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			//blending: transparencia o traslucidez
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
			mesh->RenderMesh();
			glDisable(GL_BLEND);
			posicion = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		i_aux++;
	}
}

/**
 * @brief Rotacion continua
 *
 * @param anim_rot Valor a modificar
 * @param sentido Sentido de la rotacion, solo para uso interno
 * @param min_rot Minima rotacion
 * @param max_rot Maxima rotacion
 * @param offset Paso de rotacion
 */

void rotacion_compleja_anim(GLfloat *anim_rot, bool *sentido, GLfloat min_rot, GLfloat max_rot, GLfloat offset) {
	if (sentido != NULL) {
		if (*anim_rot <= max_rot && *sentido) {
			*anim_rot += offset * deltaTime;
		}
		else if (*anim_rot >= min_rot && !*sentido) {
			*anim_rot -= offset * deltaTime;
		}
		else {
			if (*sentido) {
				*sentido = false;
				*anim_rot = max_rot;
			}
			else {
				*sentido = true;
				*anim_rot = min_rot;
			}
		}
	}
	else {
		if (max_rot > min_rot) {
			if (*anim_rot <= max_rot) {
				*anim_rot += offset * deltaTime;
			}
			else {
				*anim_rot = min_rot;
			}
		}
		else {
			if (*anim_rot <= max_rot) {
				*anim_rot -= offset * deltaTime;
			}
			else {
				*anim_rot = max_rot;
			}
		}

	}
}
/**
 * @brief Animacion simple
 *
 * @param pos Posicion a modificar
 * @param pos_final Posicion final
 * @param offset Paso de movimiento
 * @param FX Datos del archivo de efecto de sonido
 * @param status_FX Status de efecto de sonido, evita que se reproduzca mas veces de lo necesario
 */
void animacion_simple(GLfloat *pos, GLfloat pos_final, GLfloat offset, const char* FX, bool *status_FX) {
	if (FX != NULL && status_FX != NULL) {
		if (!*status_FX) {
			music->playFX(FX);
			*status_FX = true;
		}
	}
	if (pos_final < *pos) {
		if (*pos > pos_final)
			*pos -= offset * deltaTime;
	}
	else {
		if (*pos < pos_final)
			*pos += offset * deltaTime;

	}

}

void renderScene(Shader *shader) {
	uniformModel = shader->GetModelLocation();
	uniformProjection = shader->GetProjectionLocation();
	uniformView = shader->GetViewLocation();
	uniformEyePosition = shader->GetEyePositionLocation();
	uniformSpecularIntensity = shader->GetSpecularIntensityLocation();
	uniformShininess = shader->GetShininessLocation();

	GLfloat posiciones_arboles[] = {
		
		//edificios
		8.0f, 1.0f,7.0f,
		11.0f, 1.0f,7.0f,
		14.0f, 1.0f,7.0f,

		8.0f, 1.0f, 12.3f,
		11.0f, 1.0f,12.3f,
		14.0f, 1.0f,12.3f,
		//parque
		8.0f, 1.0f,-7.0f,
		11.0f, 1.0f,-7.0f,
		14.0f, 1.0f,-7.0f,

		8.0f, 1.0f, -12.3f,
		11.0f, 1.0f,-12.3f,
		14.0f, 1.0f,-12.3f,

		17.0f, 1.0f,-38.5f,
		//helipuerto
		-8.0f, 1.0f,-7.0f,
		-11.0f, 1.0f,-7.0f,
		-14.0f, 1.0f,-7.0f,

		-8.0f, 1.0f, -12.3f,
		-11.0f, 1.0f,-12.3f,
		-14.0f, 1.0f,-12.3f,

		-37.0f, 1.0f,-37.3f,
		-37.0f, 1.0f,-27.3f,
		-37.0f, 1.0f,-17.3f,

		-17.0f, 1.0f,-37.3f,
		-27.0f, 1.0f,-37.3f,
		-37.0f, 1.0f,-37.3f,

		//casa
		-8.0f, 1.0f,7.0f,
		-11.0f, 1.0f,7.0f,
		-14.0f, 1.0f,7.0f,

		-8.0f, 1.0f, 12.3f,
		-11.0f, 1.0f,12.3f,
		-14.0f, 1.0f,12.3f,
		
		-17.0f, 1.0f,19.3f,
		-27.0f, 1.0f,19.3f,
		-37.0f, 1.0f,19.3f,

		//-17.0f, 1.0f,26.3f,
		//-27.0f, 1.0f,26.3f,
		-37.0f, 1.0f,26.3f,

	};
	int num_posiciones_arboles = sizeof(posiciones_arboles) / sizeof(posiciones_arboles[0]);
	GLfloat posiciones_arbustos[] = {
		
	
		//casa
		-11.3f, 1.2f, 35.03f,
		-10.9f, 1.2f, 35.03f,
		-9.9f, 1.2f, 35.03f,
		//helipuerto
		-34.0f, 1.0f,-37.3f,
		-34.0f, 1.0f,-27.3f,
		-34.0f, 1.0f,-17.3f,

		-17.0f, 1.0f,-34.3f,
		-27.0f, 1.0f,-34.3f,
		-37.0f, 1.0f,-34.3f,
		//edificio
		7.8f, 1.2f, 30.03f,
		7.8f, 1.2f, 32.03f,
		7.8f, 1.2f, 34.03f,

		18.3f, 1.2f, 5.5f,
		19.3f, 1.2f, 5.5f,
		20.5f, 1.2f, 5.5f,
		22.5f, 1.2f, 5.5f,
		23.5f, 1.2f, 5.5f,
		24.5f, 1.2f, 5.5f,
		//parque
		18.3f, 1.2f, -5.7f,
		19.3f, 1.2f, -5.7f,
		20.5f, 1.2f, -5.7f,
		22.5f, 1.2f, -5.7f,
		23.5f, 1.2f, -5.7f,
		24.5f, 1.2f, -5.7f,

		24.0f, 1.0f,-38.5f,
		27.0f, 1.0f,-38.5f,
		34.0f, 1.0f,-38.5f,
		22.0f, 1.0f,-38.5f,
		19.0f, 1.0f,-38.5f,
		22.0f, 1.0f,-38.5f,
		30.0f, 1.0f,-38.5f,

		-20.3f, 1.5f, 20.03f,
		-25.9f, 1.5f, 20.03f,
		-30.9f, 1.5f, 20.03f,
		-35.3f, 1.5f, 20.03f,
		-40.9f, 1.5f, 20.03f,

		//-20.3f, 1.5f, 26.03f,
		//-25.9f, 1.5f, 26.03f,
		-30.9f, 1.5f, 26.03f,
		-35.3f, 1.5f, 26.03f,
		-40.9f, 1.5f, 26.03f,
		
	};
	int num_posiciones_arbustos = sizeof(posiciones_arbustos) / sizeof(posiciones_arbustos[0]);

	GLfloat posiciones_faros[] = {

		
		10.0f, 1.0f, -10.0,
		10.0f, 1.0f, 10.0,

		-10.0f, 1.0f, -10.0,
		-10.0f, 1.0f, 10.0,


		

		
	};
	int num_posiciones_faros = sizeof(posiciones_faros) / sizeof(posiciones_faros[0]);
	GLint inds_luz_faro[] = {
		1,2,3,4,//5,6,7,8,9,10,
	};
	/*GLfloat posiciones_bancas[] = {
		3.5f,1.0f,2.5f,
		14.5f,1.0f,2.5f,
		-7.0f,1.0f,2.5f,
		-18.0f,1.0f,2.5f,
	};
	int num_posiciones_bancas = sizeof(posiciones_bancas) / sizeof(posiciones_bancas[0]);*/
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(0.0f, -1.8f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	base->RenderModel();



	// Pajaro Compleja y keyframes
	modelaux = glm::mat4(1.0);
	model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-1.7f, 20.5f, 31.0f));
	model = glm::translate(model, glm::vec3(keyframes_pajaro->getVal("movX"), keyframes_pajaro->getVal("movY"), keyframes_pajaro->getVal("movZ")));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(keyframes_pajaro->getVal("giroX")), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(keyframes_pajaro->getVal("giroY")), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(keyframes_pajaro->getVal("giroZ")), glm::vec3(0.0f, 0.0f, 1.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	CuerpoPajaro->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.40f, 0.0f));
	modelaux = model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.230f));
	model = glm::rotate(model, glm::radians(mov_alas), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	AlaDer->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.230f));
	model = glm::rotate(model, glm::radians(mov_alas*(-1.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	AlaIzq->RenderModel();

	modelaux = glm::mat4(1.0);
	model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	// model = glm::translate(model, glm::vec3(-1.7f, 10.5f, 31.0f));
	model = glm::translate(model, glm::vec3(-1.7f, 10.5f, 31.0f));

	model = glm::translate(model, glm::vec3(keyframes_pajaro->getVal("movX"), keyframes_pajaro->getVal("movY"), keyframes_pajaro->getVal("movZ")));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(keyframes_pajaro->getVal("giroX")), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(keyframes_pajaro->getVal("giroY")), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(keyframes_pajaro->getVal("giroZ")), glm::vec3(0.0f, 0.0f, 1.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	CuerpoPajaro->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.40f, 0.0f));
	modelaux = model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.230f));
	model = glm::rotate(model, glm::radians(mov_alas), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	AlaDer->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.230f));
	model = glm::rotate(model, glm::radians(mov_alas*(-1.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	AlaIzq->RenderModel();



	model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Fuente.RenderModel();

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-21.5f, 3.0f, -20.5f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Helipuerto.RenderModel();
	/*
	// Helicoptero Animacion compleja
	modelaux = glm::mat4(1.0);
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-21.5f + pos_x_nave, 0.0f + pos_y_nave + func_nave, -24.5f + pos_z_helicopter));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(rot_y_nave), glm::vec3(0.0f, 1.0f, 0.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Helicoptero.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	if (pos_x_nave == 0.0f && pos_y_nave == 0.0f && pos_z_helicopter == 0.0f)
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	else
		model = glm::rotate(model, glm::radians(rot_helice), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Helice.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(-2.05f, 4.55f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	if (pos_x_nave == 0.0f && pos_y_nave == 0.0f && pos_z_helicopter == 0.0f)
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	else
		model = glm::rotate(model, glm::radians(rot_helice), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Helice_lateral.RenderModel();*/

	// Helicoptero Animacion Keyframes
	modelaux = glm::mat4(1.0);
	model = glm::mat4(1.0);
	// printf("\n%f %f %f\n", keyframes_helicoptero.getVal("movX"), keyframes_helicoptero.getVal("movY"), keyframes_helicoptero.getVal("movZ"));
	model = glm::translate(model, glm::vec3(-21.5f, 3.0f, -20.5f));
	model = glm::translate(model, glm::vec3(keyframes_helicoptero->getVal("movX"), keyframes_helicoptero->getVal("movY"), keyframes_helicoptero->getVal("movZ")));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	model = glm::rotate(model, glm::radians(keyframes_helicoptero->getVal("giroX")), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(keyframes_helicoptero->getVal("giroY")), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(keyframes_helicoptero->getVal("giroZ")), glm::vec3(0.0f, 0.0f, 1.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Helicoptero.RenderModel();

	model = modelaux;
	model = glm::translate(model, glm::vec3(0.4f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	if (!keyframes_helicoptero->getPlay())
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	else
		model = glm::rotate(model, glm::radians(rot_helice), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Helice.RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(-5.9f, 2.1f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	if (!keyframes_helicoptero->getPlay())
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	else
		model = glm::rotate(model, glm::radians(rot_helice), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Helice_lateral.RenderModel();



	

	
	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-22.0f, 0.999999999999999999f, 36.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Depa2->RenderModel();
	glDisable(GL_BLEND);


	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-37.0f, 1.1f, 38.0f));
	model = glm::scale(model, glm::vec3(0.66f, 1.0f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	picina->RenderModel();

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(27.0f, 1.6f, -27.0f));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	juego->RenderModel();

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(20.0f, 1.1f, -20.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	pasamanos->RenderModel();

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(35.0f, 1.1f, -20.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	pasamanos->RenderModel();


	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-38.0f, 0.7f, 38.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	pelota->RenderModel();


	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-15.0f, 2.0, -3.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	car->RenderModel();



	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(-25.0f+ pos_x_nave, 10.0+ pos_y_nave + func_nave,52.0f + func_nave));
	model = glm::rotate(model, glm::radians(rot_y_nave), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(-20.0f, 20.0f, -20.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	nave->RenderModel();


	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(25.0f, 1.0, -16.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Banca.RenderModel();
	model = glm::translate(modelaux, glm::vec3(0.0f, 0.0, 5.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Banca.RenderModel();
	model = glm::translate(modelaux, glm::vec3(0.0f, 0.0, 5.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Banca.RenderModel();
	model = glm::translate(modelaux, glm::vec3(0.0f, 0.0, 5.0f));
	modelaux = model;

	model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(35.0f, 1.0, -37.0f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Banca.RenderModel();





	/*model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(5.0f, 2.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	base->RenderModel();
	glDisable(GL_BLEND);
	*/


	// Avatar
	//Cabeza
	modelaux = glm::mat4(1.0);
	model = glm::mat4(1.0);
	model = glm::translate(model, posAvatar);
	model = glm::rotate(model, glm::radians(-90 + avatar_rot), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Cabeza_avatar->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.0f, -0.62f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	modelaux = model;
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	Cuerpo_avatar->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.3f, 0.35f, 0.0f));
	model = glm::rotate(model, glm::radians(rot_brazo - 45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	BrazoI_avatar->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.3f, 0.35f, 0.0f));
	model = glm::rotate(model, glm::radians(-rot_brazo), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	BrazoD_avatar->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(0.13f, -0.16f, 0.0f));

	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	PiernaI_avatar->RenderModel();
	model = modelaux;
	model = glm::translate(model, glm::vec3(-0.13f, -0.16f, 0.0f));
	model = glm::rotate(model, glm::radians(rot_pierna), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	PiernaD_avatar->RenderModel();

	loadModelArray(*Arbol, posiciones_arboles, model, uniformModel, uniformSpecularIntensity, uniformShininess, num_posiciones_arboles);
	//loadModelArray(Banca, posiciones_bancas, model, uniformModel, uniformSpecularIntensity, uniformShininess, num_posiciones_bancas);
	loadModelArrayFaro(Faro, posiciones_faros, model, uniformModel, uniformSpecularIntensity, uniformShininess, inds_luz_faro, num_posiciones_faros);
	Tagave.UseTexture();
	loadModelArbustoArray(meshList[3], posiciones_arbustos, model, uniformModel, uniformSpecularIntensity, uniformShininess, num_posiciones_arbustos);
}

int main() {
	mainWindow = Window(1920, 1080); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CrearCubo();
	CreateShaders();
	camera = Camera(glm::vec3(26.0f, 3.0f, 1.2f), glm::vec3(0.0f, 1.0f, 0.0f), 180.0f, 0.0f, 5.0f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	Tagave = Texture("Textures/arbusto.png");
	Tagave.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	std::vector<std::thread> threads;
	base = new Model();
	base->LoadModel("Models/base.obj");

	CuerpoPajaro = new Model();
	CuerpoPajaro->LoadModel("Models/pajaro_cuerpo.obj");
	AlaDer = new Model();
	AlaDer->LoadModel("Models/ala_der.obj");
	AlaIzq = new Model();
	AlaIzq->LoadModel("Models/ala_izq.obj");
	Arbol = new Model();
	Arbol->LoadModel("Models/trees.obj");
	Cabeza_avatar = new Model();
	Cabeza_avatar->LoadModel("Models/avatar_cabeza.obj");
	Cuerpo_avatar = new Model();
	Cuerpo_avatar->LoadModel("Models/avatar_cuerpo.obj");
	PiernaD_avatar = new Model();
	PiernaD_avatar->LoadModel("Models/avatar_pierna_d.obj");
	PiernaI_avatar = new Model();
	PiernaI_avatar->LoadModel("Models/avatar_pierna_i.obj");
	BrazoI_avatar = new Model();
	BrazoI_avatar->LoadModel("Models/avatar_brazo_i.obj");
	BrazoD_avatar = new Model();
	BrazoD_avatar->LoadModel("Models/avatar_brazo_d.obj");


	Faro = Model();
	Faro.LoadModel("Models/faro.obj");
	Banca = Model();
	Banca.LoadModel("Models/banca.obj");
	Fuente = Model();
	Fuente.LoadModel("Models/fuente.obj");

	Helipuerto = Model();
	Helipuerto.LoadModel("Models/helipuerto.obj");
	Helicoptero = Model();
	Helicoptero.LoadModel("Models/helicoptero2.obj");
	Helice_lateral = Model();
	Helice_lateral.LoadModel("Models/helice_la.obj");
	Helice = Model();
	Helice.LoadModel("Models/helice2.obj");



	Depa2 = new Model();
	Depa2->LoadModel("Models/depa2.obj");


	car = new Model();
	car->LoadModel("Models/car.obj");

	

	nave = new Model();
	nave->LoadModel("Models/nave.obj");

	picina = new Model();
	picina->LoadModel("Models/picina.obj");

	pelota = new Model();
	pelota->LoadModel("Models/pelota.obj");

	juego= new Model();
	juego->LoadModel("Models/parque.obj");

	pasamanos = new Model();
	pasamanos->LoadModel("Models/pasamanos.obj");


	//base= new Model();
	//base->LoadModel("Models/base.obj");
	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		-0.1f, 3.0f, 28.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[2] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[3] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[4] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[5] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[6] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[7] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[8] = PointLight(0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.03f, 0.3f, 0.01f);
	pointLightCount++;
	pointLights[9] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-2.3f, 3.0f, 28.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;
	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/hill_lf.png");
	skyboxFaces.push_back("Textures/Skybox/hill_rt.png");
	skyboxFaces.push_back("Textures/Skybox/hill_dn.png");
	skyboxFaces.push_back("Textures/Skybox/hill_up.png");
	skyboxFaces.push_back("Textures/Skybox/hill_bk.png");
	skyboxFaces.push_back("Textures/Skybox/hill_ft.png");

	std::vector<std::string> skyboxFaces_noche;
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_lf.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_rt.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_dn.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_up.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_bk.png");
	skyboxFaces_noche.push_back("Textures/Skybox/hill_noche_ft.png");

	skybox = Skybox(skyboxFaces);
	skybox_noche = Skybox(skyboxFaces_noche);
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);
	music->playMusic("sound/Popcorn.mp3");
	GLfloat now;
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose()) {
		now = float(glfwGetTime());
		deltaTime = 1.0f * (now - lastTime);
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		if (mainWindow.getCamara() == 0) {
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		if (mainWindow.getCamara() == 1) {
			camera.keyControlAerea(mainWindow.getsKeys(), deltaTime);
			camera.mouseControlAerea();
		}
		if (mainWindow.getCamara() == 2) {
			camera.setCameraPosition(glm::vec3(posAvatar.x, 15.0f, posAvatar.z));
			camera.keyControlAvatar(mainWindow.getsKeys(), deltaTime);
			camera.mouseControlAerea();
			posAvatar.x = camera.getCameraPosition().x;
			posAvatar.z = camera.getCameraPosition().z;
			keys_avatar = mainWindow.getsKeys();
			if (keys_avatar[GLFW_KEY_A] || keys_avatar[GLFW_KEY_W] || keys_avatar[GLFW_KEY_D] || keys_avatar[GLFW_KEY_S]) {
				rotacion_compleja_anim(&rot_brazo, &brazo_sentido, 0.0f, 45.0f, 50.0f);
				rotacion_compleja_anim(&rot_pierna, &pierna_sentido, -45.0f, 45.0f, 90.0f);
				if (keys_avatar[GLFW_KEY_D] && keys_avatar[GLFW_KEY_W])
					avatar_rot = 45.0f;
				else
					if (keys_avatar[GLFW_KEY_D] && keys_avatar[GLFW_KEY_S])
						avatar_rot = -45.0f;
					else
						if (keys_avatar[GLFW_KEY_S] && keys_avatar[GLFW_KEY_A])
							avatar_rot = -135.0f;
						else
							if (keys_avatar[GLFW_KEY_A] && keys_avatar[GLFW_KEY_W])
								avatar_rot = 135.0f;
							else
								if (keys_avatar[GLFW_KEY_W])
									avatar_rot = 90.0f;
								else
									if (keys_avatar[GLFW_KEY_S])
										avatar_rot = -90.0f;
									else
										if (keys_avatar[GLFW_KEY_D])
											avatar_rot = 0.0f;
										else
											if (keys_avatar[GLFW_KEY_A])
												avatar_rot = 180.0f;
			}
		}

		if (mainWindow.getCamara() == 3) {
			camera.keyControlQuiosco(mainWindow.getsKeys(), deltaTime);
			camera.mouseControlQuiosco();
		}
		// Camara en pausa
		if (mainWindow.getCamara() == 4) {

		}
		// Animacion de keyframes
		keyframes_pajaro->inputKeyframes(mainWindow.getAnimKeyPajaro());
		keyframes_pajaro->animate();
		keyframes_helicoptero->inputKeyframes(mainWindow.getAnimKeyHelicoptero());
		keyframes_helicoptero->animate();
		

		mainWindow.setAnimKeyHelicoptero(false);
		mainWindow.setAnimKeyPajaro(false);
		


		// Animacion alas de pajaro
		rotacion_compleja_anim(&mov_alas, &alas_sentido, 0.0f, 45.0f, 30.0f);
		// Animacion helices
		rotacion_compleja_anim(&rot_helice, NULL, 0, 360, 180.0f);
		if (mainWindow.getanimNave()) {
			// Altura
			if (pos_y_nave <= 15.0f) {

				pos_y_nave += movOffset * deltaTime;
			}
			else {
				if (func_sin <= 360.0f) {
					func_sin += 27.5f * deltaTime;
				}
				else
					func_sin = 0.0f;
			}
			// Recorrido helicoptero
			if (flag_helicoptero == 1) {
				if (pos_x_nave <= 30.0f) {
					pos_x_nave += movOffset * deltaTime;
					if (rot_y_nave <= 90.0f && rot_y_nave >= 0.0f)
						rot_y_nave -= giro_offset * deltaTime;
					else
						rot_y_nave = 360.0f;
				}
				else
					flag_helicoptero++;
			}
			if (flag_helicoptero == 2) {
				if (pos_z_helicopter <= 50.0f) {
					pos_z_helicopter += movOffset * deltaTime;
					if (rot_y_nave >= 270.0f)
						rot_y_nave -= giro_offset * deltaTime;
				}
				else
					flag_helicoptero++;
			}
			if (flag_helicoptero == 3) {
				if (pos_x_nave >= 0.0f) {
					pos_x_nave -= movOffset * deltaTime;
					if (rot_y_nave >= 180.0f)
						rot_y_nave -= giro_offset * deltaTime;
				}
				else
					flag_helicoptero++;
			}
			if (flag_helicoptero == 4) {
				if (pos_z_helicopter >= 0.0f) {
					pos_z_helicopter -= movOffset * deltaTime;
					if (rot_y_nave >= 90.0f)
						rot_y_nave -= giro_offset * deltaTime;
				}
				else
					flag_helicoptero = 1;
			}
			// Animacion senoidal helicoptero
			func_nave = 1.0f *(glm::sin(glm::radians(func_sin)));
		}
		else {
			rot_y_nave = 0.0f;
			func_nave = 0.0f;
			flag_helicoptero = 1;
			pos_x_nave = 0.0f;
			pos_y_nave = 0.0f;
			pos_z_helicopter = 0.0f;
		}
		//printf("posx:%f posy:%f posz:%f\n",pos_x_nave, pos_y_nave, pos_z_helicopter);
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (int(now) % 10 == 0) {
			if (int(now) != ciclos) {
				dia_flag = !dia_flag;
				ciclos = int(now);
			}
		}
		if (dia_flag == 1) {
			mainLight.setIntensity(0.3f, 0.3f);
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		else {
			mainLight.setIntensity(0.0, 0.0);
			skybox_noche.DrawSkybox(camera.calculateViewMatrix(), projection);
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		if (mainWindow.OnOffLight())
			spotLights[0].SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		else
			spotLights[0].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		if (mainWindow.statusluzR())
			spotLights[1].SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
		else
			spotLights[1].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		if (mainWindow.statusluzG())
			spotLights[2].SetColor(glm::vec3(0.0f, 1.0f, 0.0f));
		else
			spotLights[2].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		if (mainWindow.statusluzB())
			spotLights[3].SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
		else
			spotLights[3].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
		//spotLights[1].SetDirection(glm::vec3(posSpotX[0], posSpotY[0], posSpotZ[0]));
		//spotLights[2].SetDirection(glm::vec3(posSpotX[1], posSpotY[1], posSpotZ[1]));
		//spotLights[3].SetDirection(glm::vec3(posSpotX[2], posSpotY[2], posSpotZ[2]));
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount); // Luces principales 
		shaderList[0].SetSpotLights(spotLights, spotLightCount); // "Linterna"
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		//printf("%f %f %f\n", camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
		renderScene(&shaderList[0]);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}