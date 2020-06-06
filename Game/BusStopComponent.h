#ifndef BUSSTOPCOMPONENT_H_INCLUDED
#define BUSSTOPCOMPONENT_H_INCLUDED


#include <string>
#include <cmath>

#include "../Scene/Component.h"

#include "../Bus/Bus.h"


class BusStopComponent : public Component
{
    friend class BusStopSystem;

    private:
        std::string _name;
        int _id;
        std::string _announcementFileName;
        RSound* _announcementSound;
        bool _announcementIsPlay;
        unsigned int _numberOfPassengers;

        float _time;

    public:
        BusStopComponent(std::string name)
            : Component(CT_BUS_STOP),
            _name(name), _numberOfPassengers(0), _time(0.0f),
            _announcementIsPlay(false)
        {

        }

        ~BusStopComponent()
        {

        }

        void setNumberOfPassengers(unsigned int numberOfPassengers)
        {
            _numberOfPassengers = numberOfPassengers;
        }

        unsigned int getNumberOfPassengers()
        {
            return _numberOfPassengers;
        }

        void setName(std::string name)
        {
            _name = name;
        }

        std::string getName()
        {
            return _name;
        }

        void setId(int id)
        {
            _id = id;
        }

        int getId()
        {
            return _id;
        }

        void setAnnouncementFileName(std::string announcementFileName);

        std::string getAnnouncementFileName()
        {
            return _announcementFileName;
        }

        bool getAnnouncementIsPlay()
        {
            return _announcementIsPlay;
        }

        void setAnnouncementIsPlay(bool announcementIsPlay)
        {
            _announcementIsPlay = announcementIsPlay;
        }

        RSound* getAnnouncementSound()
        {
            return _announcementSound;
        }

        void onTrigger(float deltaTime, Bus* bus)
        {
            unsigned int openedDoors = 0;
            for (int i = 0; i < bus->getDoorsCount(); ++i)
            {
                if (bus->getDoor(i)->getState() == EDS_OPENING)
                    ++openedDoors;
            }

            if (openedDoors > 0)
            {
                _time += deltaTime;
                while (_time > 1.0f && bus->getNumberOfPassengersGettingOff() > 0)
                {
                    int passangers = std::min(openedDoors, bus->getNumberOfPassengersGettingOff());
                    bus->setNumberOfPassengers(bus->getNumberOfPassengers() - passangers);
                    bus->setNumberOfPassengersGettingOff(bus->getNumberOfPassengersGettingOff() - passangers);

                    _time -= 1.0f;
                }

                while (_time > 1.0f && _numberOfPassengers > 0)
                {
                    int passangers = std::min(openedDoors, _numberOfPassengers);
                    bus->setNumberOfPassengers(bus->getNumberOfPassengers() + passangers);
                    _numberOfPassengers -= passangers;

                    _time -= 1.0f;
                }
            }
        }

};


#endif // BUSSTOPCOMPONENT_H_INCLUDED
