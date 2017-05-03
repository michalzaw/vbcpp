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
        : _camera(0), _device(0), _context(0)
        {
            std::cout << "SoundManager: Konstruktor\n";
            alutInit(NULL, NULL);

            _device = alcOpenDevice(NULL);
            _context = alcCreateContext(_device, NULL);

            /* listener at origin, facing down -z (ears at 1.5m height) */
            alListener3f( AL_POSITION, 0., 1.5, 0. );
            alListener3f( AL_VELOCITY, 0., 0., 0. );
            float orient[6] = { /*fwd:*/ 0., 0., -1., /*up:*/ 0., 1., 0. };
            alListenerfv( AL_ORIENTATION, orient );
        }

        virtual ~SoundManager()
        {
            std::cout << "SoundManager: Destructor\n";

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

                alListenerf(AL_VELOCITY, 1.0f);

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

                    if (distance <= playDistance)
                        (*i)->play();
                    else
                        (*i)->stop();
                }
            }
        }

        SoundComponent* addSoundComponent(SoundComponent* soundcomp)
        {
            _sounds.push_back(soundcomp);

            return soundcomp;
        }

    protected:
        CameraStatic*   _camera;
        ALCdevice *_device;
        ALCcontext *_context;

        std::list<SoundComponent*>  _sounds;
};


#endif // SOUNDMANAGER_H_INCLUDED
