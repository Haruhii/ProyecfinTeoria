#include "Sound.h"


Sound::Sound()
{}
Sound::Sound(float posx, float posy, float posz){
	pos_x = posx;
	pos_y = posy;
	pos_z = posz;
	if (!engine)
		printf("Error en cargar el motor de audio\n");
}

void Sound::playMusic(const char* file) {
	engine->setSoundVolume(0.5f);
	music = engine->play3D(file, vec3df(pos_x, pos_y, pos_z), true, false, false);
}
void Sound::playFX(const char* file) {
	music = engine->play3D(file, vec3df(pos_x, pos_y, pos_z), false, false, true);
}

Sound::~Sound()
{}