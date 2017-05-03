#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED


/* OpenAL headers */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "Component.h"


typedef enum SoundType
{
    EST_PLAYER = 0,
    EST_AMBIENT
};


class SoundComponent : virtual public Component
{
    public:
        SoundComponent(std::string file, SoundType type, bool looping = false)
        : _buffer(0), _source(0), Component(CT_SOUND), _play(false), _looping(looping), _type(type), _playDistance(10.0f)
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

        }


        glm::vec3 getPosition()
        {
            return _object->getPosition();
        }


        float getPlayDistance()
        {
            return _playDistance;
        }


        void play()
        {
            ALenum state;

            alGetSourcei(_source, AL_SOURCE_STATE, &state);

            std::cout << _source << "PLAY" << std::endl;

            //_play = true;

            if (state != AL_PLAYING)
                alSourcePlay(_source);
        }

        void stop()
        {
            alSourceStop(_source);
            //_play = false;
        }


        SoundType getSoundType()
        {
            return _type;
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

        SoundType   _type;
        float       _playDistance;
};


#endif // SOUNDCOMPONENT_H_INCLUDED
