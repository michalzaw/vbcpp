#ifndef BUSSTOPCOMPONENT_H_INCLUDED
#define BUSSTOPCOMPONENT_H_INCLUDED


#include <string>
#include <cmath>

#include "../../Scene/Component.h"

#include "../../Bus/Bus.h"


class BusStopComponent : public Component
{
    VBCPP_COMPONENT(BusStopComponent, CT_BUS_STOP)

    friend class TransitSystem;

    private:
        static const int MAX_NUMBER_OF_PASSENGERS = 30;

        std::string _name;
        int _id;
        std::string _announcementFileName;
        float _distance;
        bool _requestStop;
        unsigned int _numberOfPassengersMin;
        unsigned int _numberOfPassengersMax;
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
            setNumberOfPassengers(rand() % MAX_NUMBER_OF_PASSENGERS);
        }

        ~BusStopComponent()
        {

        }

        LUAF void setNumberOfPassengers(unsigned int numberOfPassengers)
        {
            _numberOfPassengers = numberOfPassengers;
        }

        LUAF unsigned int getNumberOfPassengers()
        {
            return _numberOfPassengers;
        }

        LUAF void setName(std::string name)
        {
            _name = name;
        }

        LUAF std::string getName()
        {
            return _name;
        }

        LUAF void setId(int id)
        {
            _id = id;
        }

        LUAF int getId()
        {
            return _id;
        }

        LUAF void setAnnouncementFileName(std::string announcementFileName);

        LUAF std::string getAnnouncementFileName()
        {
            return _announcementFileName;
        }

        LUAF void setDistance(float distance)
        {
            _distance = distance;
        }

        LUAF float getDistance()
        {
            return _distance;
        }

        LUAF void setRequestStop(bool requestStop)
        {
            _requestStop = requestStop;
        }

        LUAF bool isRequestStop()
        {
            return _requestStop;
        }

        LUAF void setNumberOfPassengersMin(unsigned int numberOfPassengersMin)
        {
            _numberOfPassengersMin = numberOfPassengersMin;
        }

        LUAF unsigned int getNumberOfPassengersMin()
        {
            return _numberOfPassengersMin;
        }

        LUAF void setNumberOfPassengersMax(unsigned int numberOfPassengersMax)
        {
            _numberOfPassengersMax = numberOfPassengersMax;
        }

        LUAF unsigned int getNumberOfPassengersMax()
        {
            return _numberOfPassengersMax;
        }

        LUAF bool getAnnouncementIsPlay()
        {
            return _announcementIsPlay;
        }

        LUAF void setAnnouncementIsPlay(bool announcementIsPlay)
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
