#ifndef LOADTEXTURE_H_INCLUDED
#define LOADTEXTURE_H_INCLUDED


#include <iostream>

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include "../Utils/RTexture2D.h"
#include "../Utils/RTextureCubeMap.h"


// U¿ycie SOIL jedynie do wczytania pliku graficznego
// Tworzenie tekstury wykonywane recznie, gdyz wbudowane funkcje SOILa powoduja crash na Windowsie
//GLuint loadTexture(const char* fileName, int* width, int* height, bool mipmaping = true);
RTexture2D* loadTexture(const char* fileName, bool mipmaping = true, RTexture2D* oldTexture = NULL);

// filesNames: pos_x, neg_x, pos_y, neg_y, pos_z, neg_z
RTextureCubeMap* loadTextureCubeMap(std::string* filesNames, const char* path, bool mipmaping = true, RTextureCubeMap* oldTexture = NULL);


GLuint loadTextureRect(const char* fileName);


#endif // LOADTEXTURE_H_INCLUDED
