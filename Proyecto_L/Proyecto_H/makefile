SRCDIR=$(PWD)
NAMEDEST=ventana
SRCDISTNAME=main.cpp
CLASSES=camera lights model mesh sphere window material shader texture skybox keyframe sound
SRCCODE=Camera.cpp Sound.cpp Light.cpp Mesh_tn.cpp Sphere.cpp Window.cpp DirectionalLight.cpp Keyframe.cpp Material.cpp PointLight.cpp Shader_light.cpp Texture.cpp
CC=g++
CFLAGS=-std=c++2a -Wall -lGL -lGLEW -lglfw -lassimp -lIrrKlang
INCLUDES=-I$(SRCDIR)/glm -I$(SRCDIR)/include
NPROCS = $(shell grep -c 'processor' /proc/cpuinfo)
MAKEFLAGS += -j$(NPROCS)
.PHONY: all
all: $(CLASSES)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAMEDEST) $(SRCDISTNAME) *.o
objects.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRCCODE)
.PHONY: material
material:
	$(CC) $(CFLAGS) $(INCLUDES) -c Material.cpp
.PHONY: skybox
skybox:
	$(CC) $(CFLAGS) $(INCLUDES) -c Skybox.cpp
.PHONY: window
window:
	$(CC) $(CFLAGS) $(INCLUDES) -c Window.cpp
.PHONY: lights
lights:light light_child
light_child:
	$(CC) $(CFLAGS) $(INCLUDES) -c PointLight.cpp DirectionalLight.cpp SpotLight.cpp
light:
	$(CC) $(CFLAGS) $(INCLUDES) -c Light.cpp
.PHONY: camera
camera:
	$(CC) $(CFLAGS) $(INCLUDES) -c Camera.cpp
.PHONY: mesh
mesh: 
	$(CC) $(CFLAGS) $(INCLUDES) -c Mesh_tn.cpp
.PHONY: keyframe
keyframe:
	$(CC) $(CFLAGS) $(INCLUDES) -c Keyframe.cpp
.PHONY: sound
sound:
	$(CC) $(CFLAGS) $(INCLUDES) -c Sound.cpp
.PHONY: texture
texture:
	$(CC) $(CFLAGS) $(INCLUDES) -c Texture.cpp
.PHONY: shader
shader:
	$(CC) $(CFLAGS) $(INCLUDES) -c Shader_light.cpp
.PHONY: sphere
sphere:
	$(CC) $(CFLAGS) $(INCLUDES) -c Sphere.cpp
.PHONY: model
model:
	$(CC) $(CFLAGS) $(INCLUDES) -c Model.cpp
.PHONY: dist
dist:
	$(CC) $(CFLAGS) $(INCLUDES) -o $(NAMEDEST) $(SRCDISTNAME) *.o
.PHONY: run
run:
	chmod +x $(NAMEDEST)
	./$(NAMEDEST)
.PHONY: clean
clean:
	rm -f *.o