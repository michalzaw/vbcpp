#include "SoundLoader.h"


RSound* loadSound(const char* fileName)
{
    ALuint buffer = alutCreateBufferFromFile(fileName);

    //std::cout << "??? SoundComponend - Constructor" << std::endl;
    //std::cout << "Buffer" << _buffer << std::endl;

    if (buffer == AL_NONE)
    {
        ALenum error = alutGetError ();
        fprintf (stderr, "Error loading file: '%s'\n",
        alutGetErrorString (error));
    }

    RSound* sound = new RSound(fileName, buffer);
    return sound;
}
