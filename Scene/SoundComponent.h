#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED


/* OpenAL headers */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "Component.h"

#include "../Utils/RSound.h"


enum SoundType
{
    EST_PLAYER = 0,
    EST_AMBIENT
};

/**
    Sound component - config file attributes:

    @param file - sound filename to be played (example: file="sound.wav")
    @param looping - tells whether sound should be looped or not. Values are "true" or "false". Default is "false".
    @param playDistance - set max distance from the sound can be heard. Default value is 10.0f.
    @param volume - sets volume of the sound. It's in range 0.0 - 1.0.
    @param position - sets sound's offset relatively to scene object. Default is (0,0,0) which means sound has it's origin in the scene object's origin.
**/


class SoundComponent : virtual public Component
{
    public:
        SoundComponent(RSound* sound, SoundType type, bool looping = false)
        : Component(CT_SOUND), _source(0), _sound(sound), _looping(looping), _mute(false), _play(false), _soundPosition(0,0,0), _type(type), _playDistance(10.0f)
        {
            alGenSources( 1, &_source );

            if (_object)
            {
                update();

                //glm::vec3 pos = _object->getPosition();

                //alSource3f( _source, AL_POSITION, pos.x, pos.y, pos.z );
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

            alSourcei( _source, AL_REFERENCE_DISTANCE, 1.0f); // 1.0f
            alSourcei( _source, AL_MAX_DISTANCE, _playDistance);  // 1000.0f

            alSourcei(_source, AL_BUFFER, _sound->getBuffer());
        }

        virtual ~SoundComponent()
        {

            alDeleteSources(1, &_source);
        }

        ALuint getSource()
        {
                return _source;
        }

        void update()
        {
            if (_object)
            {
                glm::vec3 pos = _object->getPosition() + _soundPosition;

                alSource3f( _source, AL_POSITION, pos.x, pos.y, pos.z );
            }

        }

        void setPosition(glm::vec3 newPos)
        {
            _soundPosition = newPos;

            update();
        }

        glm::vec3 getPosition()
        {
            if (_object)
                return _object->getPosition() + _soundPosition;
            else
                return _soundPosition;
        }

        const float getPlayDistance() const
        {
            return _playDistance;
        }

        void setPlayDistance(const float distance)
        {
            _playDistance = distance;
            alSourcei( _source, AL_MAX_DISTANCE, _playDistance);
        }

        void play()
        {
            ALenum state;

            alGetSourcei(_source, AL_SOURCE_STATE, &state);

            if (state != AL_PLAYING && !_mute)
                alSourcePlay(_source);
        }

        void stop()
        {
            if (!_mute)
                alSourceStop(_source);
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

        void setMute(const bool mute)
        {
            _mute = mute;
        }

    protected:
        ALuint  _source;
        RSound* _sound;
        bool    _looping;
        bool    _mute;

        bool    _play;

        glm::vec3   _soundPosition;

        SoundType   _type;
        float       _playDistance;
};


#endif // SOUNDCOMPONENT_H_INCLUDED
