#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED


/* OpenAL headers */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "Component.h"

class SoundComponent : virtual public Component
{
    public:
        SoundComponent(std::string file, bool looping = false)
        : _buffer(0), _source(0), Component(CT_SOUND), _play(false), _looping(looping)
        {
        std::cout << "Engine sound: " << file << std::endl;
            alGenSources( 1, &_source );

            _buffer =alutCreateBufferFromFile (file.c_str());

            std::cout << "??? SoundComponend - Constructor" << std::endl;
            std::cout << "Buffer" << _buffer << std::endl;

            if (_buffer == AL_NONE)
            {
                ALenum error = alutGetError ();
                fprintf (stderr, "Error loading file: '%s'\n",
                alutGetErrorString (error));
            }
            else
            {
                if (_object)
                {
                    glm::vec3 pos = _object->getPosition();

                    alSource3f( _source, AL_POSITION, pos.x, pos.y, pos.z );
                    alSource3f( _source, AL_VELOCITY, 0., 0., 0. );
                }

                else
                {
                    fprintf (stderr, "Sound component not attached to any Scene Object! Using default values\n");


                    alSource3f( _source, AL_POSITION, 0., 1., 0. );
                    alSource3f( _source, AL_VELOCITY, 0., 0., 0. );
                }

                alSourcef( _source, AL_PITCH, 1.0f );
                alSourcef( _source, AL_GAIN, 1. );
                alSourcei( _source, AL_LOOPING, looping );

                alSourcei(_source, AL_BUFFER, _buffer);
            }
        }

        virtual ~SoundComponent()
        {

            alDeleteSources(1, &_source);
            alDeleteBuffers(1, &_buffer);
        }

        ALuint getSource()
        {
                return _source;
        }

        void update()
        {
            if (_object)
            {
                glm::vec3 pos = _object->getPosition();

                alSource3f( _source, AL_POSITION, pos.x, pos.y, pos.z );
            }

            if (_play)
            {
                ALenum state;

                alGetSourcei(_source, AL_SOURCE_STATE, &state);


                //if (state != AL_INITIAL)
                    //alSourcePlay(_source);
                    //_play = false;
                //else if (state == AL_STOPPED)
                    //_play = false;

            }
            //else
                //alSourceStop(_source);
        }

        void play()
        {
            //_play = true;
            alSourcePlay(_source);
        }

        void stop()
        {
            alSourceStop(_source);
            //_play = false;
        }

        void setGain(ALfloat gain)
        {
            alSourcef( _source, AL_GAIN, gain );
        }

        void setPitch(ALfloat pitch)
        {
            alSourcef( _source, AL_PITCH, pitch );
        }

    protected:
        ALuint  _source;
        ALuint  _buffer;
        bool    _looping;

        bool    _play;
};


#endif // SOUNDCOMPONENT_H_INCLUDED
