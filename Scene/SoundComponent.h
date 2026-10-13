#ifndef SOUNDCOMPONENT_H_INCLUDED
#define SOUNDCOMPONENT_H_INCLUDED


/* OpenAL headers */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>

#include "Component.h"
#include "SceneObject.h" // todo: usunac po podzieleniu na h i cpp

#include "../Scripting/Utils/LuaMacros.h"

#include "../Utils/RSound.h"


enum SoundType
{
    EST_PLAYER = 0,
    EST_AMBIENT,

    EST_COUNT
};


const std::string soundTypeStrings[] = { "player", "ambient" };
inline SoundType getSoundTypeFromString(const std::string& name)
{
    for (int i = 0; i < EST_COUNT; ++i)
    {
        if (soundTypeStrings[i] == name)
            return static_cast<SoundType>(i);
    }
}


/**
    Sound component - config file attributes:

    @param file - sound filename to be played (example: file="sound.wav")
    @param looping - tells whether sound should be looped or not. Values are "true" or "false". Default is "false".
    @param playDistance - set max distance from the sound can be heard. Default value is 10.0f.
    @param volume - sets volume of the sound. It's in range 0.0 - 1.0.
    @param position - sets sound's offset relatively to scene object. Default is (0,0,0) which means sound has it's origin in the scene object's origin.
**/


class SoundComponent : public Component
{
    VBCPP_COMPONENT(SoundComponent, CT_SOUND)

    public:
        SoundComponent(RSound* sound, SoundType type, bool looping = false)
        : Component(CT_SOUND), _source(0), _sound(sound), _looping(looping), _mute(false), _pitch(1.0f), _gain(1.0f), _play(false), _soundPosition(0,0,0), _type(type), _playDistance(10)
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
                LOG_WARNING("Sound component not attached to any Scene Object! Using default values");


                alSource3f( _source, AL_POSITION, 0., 1., 0. );
                alSource3f( _source, AL_VELOCITY, 0., 0., 0. );
            }

            alSourcef( _source, AL_PITCH, _pitch );
            alSourcef( _source, AL_GAIN, _gain );
            alSourcei( _source, AL_LOOPING, looping );

            alSourcei( _source, AL_REFERENCE_DISTANCE, 1); // 1.0f
            alSourcei( _source, AL_MAX_DISTANCE, _playDistance);  // 1000.0f

            if (_sound != NULL)
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
                glm::vec3 pos = _object->getGlobalPosition() + _soundPosition;

                alSource3f( _source, AL_POSITION, pos.x, pos.y, pos.z );
            }

        }

        LUAF void setPosition(glm::vec3 newPos)
        {
            _soundPosition = newPos;

            update();
        }

        LUAF glm::vec3 getPosition()
        {
            return _soundPosition;
        }

        LUAF glm::vec3 getGlobalPosition()
        {
            if (_object)
                return _object->getGlobalPosition() + _soundPosition;
            else
                return _soundPosition;
        }

        LUAF const ALint getPlayDistance() const
        {
            return _playDistance;
        }

        LUAF void setPlayDistance(const ALint distance)
        {
            _playDistance = distance;
            alSourcei( _source, AL_MAX_DISTANCE, _playDistance);
        }

        LUAF void play()
        {
            ALenum state;

            alGetSourcei(_source, AL_SOURCE_STATE, &state);

            if (state != AL_PLAYING && !_mute)
                alSourcePlay(_source);
        }

        LUAF void stop()
        {
            if (!_mute)
                alSourceStop(_source);
        }

        LUAF ALenum getState()
		{
			ALenum state;
			alGetSourcei(_source, AL_SOURCE_STATE, &state);

			return state;
		}


        void setSoundType(SoundType type)
        {
            _type = type;
        }

        SoundType getSoundType()
        {
            return _type;
        }

        LUAF void setGain(ALfloat gain)
        {
            _gain = gain;

            alSourcef( _source, AL_GAIN, gain );
        }

        LUAF float getGain()
        {
            return _gain;
        }

        LUAF void setPitch(ALfloat pitch)
        {
            _pitch = pitch;

            alSourcef( _source, AL_PITCH, pitch );
        }

        LUAF float getPitch()
        {
            return _pitch;
        }

        LUAF void setMute(const bool mute)
        {
            _mute = mute;
        }

        void setSound(RSound* sound)
        {
            _sound = sound;

            alSourcei(_source, AL_BUFFER, _sound->getBuffer());
        }

        RSound* getSound()
        {
            return _sound;
        }

        LUAF void setLooping(bool looping)
        {
            _looping = looping;

            alSourcei(_source, AL_LOOPING, looping);
        }

        LUAF bool isLooping()
        {
            return _looping;
        }

    protected:
        ALuint  _source;
        RSound* _sound;
        bool    _looping;
        bool    _mute;

        float _pitch;
        float _gain;

        bool    _play;

        glm::vec3   _soundPosition;

        SoundType   _type;
        ALint       _playDistance;
};


#endif // SOUNDCOMPONENT_H_INCLUDED
