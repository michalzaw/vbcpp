#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <string>
#include <vector>

struct point
{
    point() : rpm(0), torque(0) { }

    float rpm;
    float torque;
};

struct params
{
    params() : a(0), b(0) { }

    float a;
    float b;
};


class Engine
{
    public:
        Engine(const std::string filename);
        virtual ~Engine();

        void turnOn();
        void turnOff();

        float getThrottle() { return _throttle; }

        float getCurrentRPM() { return _currentRPM; }

        void setRPM(float wheelAngularVelocity, float gearRatio);

        bool isRunning() { return _isRunning; }

        void setThrottle(float throttle) { _throttle = throttle; }

        void throttleUp();

        void throttleDown();

        float getCurrentTorque() { return _currentTorque; }

        std::string getSoundFilename() { return _sound; }

        float getSoundVolume() { return _volume; }

        void update(float dt);


    protected:
        bool _isRunning;

        float   _throttle;
        float   _currentRPM;
        float   _currentTorque;
        float   _maxRPM;

        std::string _sound;
        float       _volume;

        std::vector<point>  _torqueCurve;
        std::vector<params> _curveParams;

        void loadData(const std::string filename);
        void calculateTorqueLine();
        float getMaxTorque();
};

#endif // ENGINE_H_INCLUDED
