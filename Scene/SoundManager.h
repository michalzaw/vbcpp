#ifndef SOUNDMANAGER_H_INCLUDED
#define SOUNDMANAGER_H_INCLUDED

#include "../Utils/RefCounter.h"
#include "SoundComponent.h"

/* OpenAL headers */
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <AL/alut.h>


#include <list>

#include "../Graphics/CameraStatic.hpp"


class SoundManager : virtual public RefCounter
{
    public:
        SoundManager()
        : _camera(0), _device(0), _context(0), _mute(false)
        {
            LOG_INFO("SoundManager: Konstruktor");
            alutInit(NULL, NULL);

            _device = alcOpenDevice(NULL);
            _context = alcCreateContext(_device, NULL);

            /* listener at origin, facing down -z (ears at 1.5m height) */
            alListener3f( AL_POSITION, 0., 1.5, 0. );
            alListener3f( AL_VELOCITY, 0., 0., 0. );
            float orient[6] = { /*fwd:*/ 0., 0., -1., /*up:*/ 0., 1., 0. };
            alListenerfv( AL_ORIENTATION, orient );

            alListenerf( AL_GAIN, 0.9f);

            alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
        }

        virtual ~SoundManager()
        {
            LOG_INFO("SoundManager: Destructor");

            for (std::list<SoundComponent*>::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
            {
                delete *i;
            }

            _device = alcGetContextsDevice(_context);
            alcMakeContextCurrent(NULL);
            alcDestroyContext(_context);
            alcCloseDevice(_device);

            alutExit ();
        }


        void setActiveCamera(CameraStatic* camera)
        {
            _camera = camera;
        }


        void update()
        {



            if (_camera)
            {
                glm::vec3 cameraPos = _camera->getPosition();
                alListener3f( AL_POSITION, cameraPos.x, cameraPos.y, cameraPos.z );

                glm::vec3 right = _camera->getRightVector();
                glm::vec3 up = _camera->getUpVector();

                glm::vec3 fwd = glm::cross(up, right);

                alListenerf(AL_METERS_PER_UNIT, 1.0f);

                float orient[6] = { /*fwd:*/ fwd.x, fwd.y, fwd.z, /*up:*/ up.x, up.y, up.z };
                alListenerfv( AL_ORIENTATION, orient );
            }


            for (std::list<SoundComponent*>::iterator i = _sounds.begin(); i != _sounds.end(); ++i)
            {
                (*i)->update();

                if ((*i)->getSoundType() == EST_AMBIENT)
                {
                    //std::cout << "AMBIENT SOUND - UPDATE" << std::endl;

                    glm::vec3 cameraPos;

                    if (_camera)
                        cameraPos = _camera->getPosition();

                    glm::vec3 objPos = (*i)->getPosition();
                    float playDistance = (*i)->getPlayDistance();

                    float distance = glm::distance(cameraPos, objPos);

                    if (distance <= playDistance && !_mute)
                        (*i)->play();
                    else
                        (*i)->stop();
                }
            }
        }

        void setMute(const bool mute)
        {
            _mute = mute;

            for (SoundComponent* sound : _sounds)
            {
                sound->setMute(_mute);
            }
        }

        SoundComponent* addSoundComponent(SoundComponent* soundcomp)
        {
            _sounds.push_back(soundcomp);
            soundcomp->setMute(_mute);

            return soundcomp;
        }

        void removeSoundComponent(SoundComponent* soundComponent)
        {
            _sounds.remove(soundComponent);

            delete soundComponent;
        }

    protected:
        CameraStatic*   _camera;
        ALCdevice*      _device;
        ALCcontext*     _context;

        bool _mute;

        std::list<SoundComponent*>  _sounds;
};


#endif // SOUNDMANAGER_H_INCLUDED
