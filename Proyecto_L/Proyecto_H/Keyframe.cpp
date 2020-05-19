#include "Keyframe.h"
Keyframe::Keyframe() {

}

Keyframe::Keyframe(const char* keyFile, int _numFrames, const char* _id_key) {
	nameFile = keyFile;
	numFrames = _numFrames;
	id_keyFrames = _id_key;
	parse_key_frames();
}


void Keyframe::parse_key_frames() {
	int ibuff, maxbuff = 0;
	char buffer[10];
	float fbuff;
	#ifdef __unix__
	kFile = fopen64(nameFile, "r");
	#elif _WIN64 || _WIN32
	fopen_s(&kFile, nameFile, "r");
	#endif
	if (!kFile) {
		printf("Error al abrir el archivo %s\n", nameFile);
		return;
	}
	rewind(kFile);
	while (!feof(kFile)) {
		#ifdef __unix__
		fscanf(kFile, "%d",&ibuff);
		#elif _WIN64 || _WIN32
		fscanf_s(kFile, "%d",&ibuff);
		#endif
		if (ibuff > maxbuff) {
			maxbuff = ibuff;
		}
		#ifdef __unix__
		fscanf(kFile, "%s", buffer, 10);
		fscanf(kFile, "%f", &fbuff);
		#elif _WIN64 || _WIN32
		fscanf_s(kFile, "%s", buffer, 10);
		fscanf_s(kFile, "%f", &fbuff);
		#endif
		if (strcmp("mov_x", buffer) == 0) {
			KeyFrame[ibuff].mov_x = fbuff;
		}
		if (strcmp("mov_y", buffer) == 0) {
			KeyFrame[ibuff].mov_y = fbuff;
		}
		if (strcmp("mov_z", buffer) == 0) {
			KeyFrame[ibuff].mov_z = fbuff;
		}
		if (strcmp("giro_x", buffer) == 0) {
			KeyFrame[ibuff].giro_x = fbuff;
		}
		if (strcmp("giro_y", buffer) == 0) {
			KeyFrame[ibuff].giro_y = fbuff;
		}
		if (strcmp("giro_z", buffer) == 0) {
			KeyFrame[ibuff].giro_z = fbuff;
		}
		if (strcmp("scale_x", buffer) == 0) {
			KeyFrame[ibuff].scale_x = fbuff;
		}
		if (strcmp("scale_y", buffer) == 0) {
			KeyFrame[ibuff].scale_y = fbuff;
		}
		if (strcmp("scale_z", buffer) == 0) {
			KeyFrame[ibuff].scale_z = fbuff;
		}
	}
	FrameIndex = maxbuff + 1;
	fclose(kFile);
}
void Keyframe::animate(){
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d key_id:%s\n", playIndex, id_keyFrames);
			printf("movX= %f\nmovY= %f\nmovZ= %f\nGiroX= %f\nGiroY= %f\nGiroZ= %f\n", mov_x, mov_y, mov_z, giro_x, giro_y, giro_z);
			if (playIndex > FrameIndex - 2)	
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			mov_x += KeyFrame[playIndex].mov_xInc;
			mov_y += KeyFrame[playIndex].mov_yInc;
			mov_z += KeyFrame[playIndex].mov_zInc;
			giro_x += KeyFrame[playIndex].giro_xinc;
			giro_y += KeyFrame[playIndex].giro_yinc;
			giro_z += KeyFrame[playIndex].giro_zinc;
			scale_x += KeyFrame[playIndex].scale_xinc;
			scale_y += KeyFrame[playIndex].scale_yinc;
			scale_z += KeyFrame[playIndex].scale_zinc;
			i_curr_steps++;
		}
	}
}

void Keyframe::interpolation(){
	interpolate(&KeyFrame[playIndex].mov_xInc, &KeyFrame[playIndex + 1].mov_x, &KeyFrame[playIndex].mov_x);
	interpolate(&KeyFrame[playIndex].mov_yInc, &KeyFrame[playIndex + 1].mov_y, &KeyFrame[playIndex].mov_y);
	interpolate(&KeyFrame[playIndex].mov_zInc, &KeyFrame[playIndex + 1].mov_z, &KeyFrame[playIndex].mov_z);
	interpolate(&KeyFrame[playIndex].giro_xinc, &KeyFrame[playIndex + 1].giro_x, &KeyFrame[playIndex].giro_x);
	interpolate(&KeyFrame[playIndex].giro_yinc, &KeyFrame[playIndex + 1].giro_y, &KeyFrame[playIndex].giro_y);
	interpolate(&KeyFrame[playIndex].giro_zinc, &KeyFrame[playIndex + 1].giro_z, &KeyFrame[playIndex].giro_z);
	interpolate(&KeyFrame[playIndex].scale_xinc, &KeyFrame[playIndex + 1].scale_x, &KeyFrame[playIndex].scale_x);
	interpolate(&KeyFrame[playIndex].scale_yinc, &KeyFrame[playIndex + 1].scale_y, &KeyFrame[playIndex].scale_y);
	interpolate(&KeyFrame[playIndex].scale_zinc, &KeyFrame[playIndex + 1].scale_z, &KeyFrame[playIndex].scale_z);
}

void Keyframe::interpolate(float *inc,float *nVal, float *val) {
	*inc = (*nVal - *val)/i_max_steps;
}

void Keyframe::resetElements(){
	mov_x = KeyFrame[0].mov_x;
	mov_y = KeyFrame[0].mov_y;
	mov_z = KeyFrame[0].mov_z;
	giro_x = KeyFrame[0].giro_x;
	giro_y = KeyFrame[0].giro_y;
	giro_z = KeyFrame[0].giro_z;
	scale_x = KeyFrame[0].scale_x;
	scale_y = KeyFrame[0].scale_y;
	scale_z = KeyFrame[0].scale_z;
}


float Keyframe::getVal(const char* val){
	if (strcmp("movX", val) == 0) {
		return mov_x;
	}
	if (strcmp("movY", val) == 0) {
		return mov_y;
	}
	if (strcmp("movZ", val) == 0) {
		return mov_z;
	}
	if (strcmp("giroX", val) == 0) {
		return giro_x;
	}
	if (strcmp("giroY", val) == 0) {
		return giro_y;
	}
	if (strcmp("giroZ", val) == 0) {
		return giro_z;
	}
	if (strcmp("scaleX", val) == 0) {
		return scale_x;
	}
	if (strcmp("scaleX", val) == 0) {
		return scale_y;
	}
	if (strcmp("scaleX", val) == 0) {
		return scale_z;
	}
	else {
		printf("no existe ese Keyframe\n");
		return NULL;
	}
}
void Keyframe::inputKeyframes(bool accion){
	if (accion){
		if (play == false){
			parse_key_frames();
			resetElements();
			//First Interpolation				
			interpolation();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}else{
			play = false;
		}
	}
}

Keyframe::~Keyframe(){
	
}