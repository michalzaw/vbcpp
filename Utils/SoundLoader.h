#ifndef SOUNDLOADER_H_INCLUDED
#define SOUNDLOADER_H_INCLUDED


#include <cstdio>

/* OpenAL headers */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "RSound.h"


RSound* loadSound(const char* fileName);


#endif // SOUNDLOADER_H_INCLUDED
