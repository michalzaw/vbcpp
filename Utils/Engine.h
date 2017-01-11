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

        bool isRunning() { return _isRunning; }

        void setThrottle(float throttle) { _throttle = throttle; }

        float getCurrentTorque() { return _currentTorque; }

        void update();

    protected:
        //bool _clutch;
        bool _isRunning;

        float   _throttle;
        float   _currentRPM;
        float   _currentTorque;
        float   _maxRPM;

        std::vector<point>  _torqueCurve;
        std::vector<params> _curveParams;

        void loadData(const std::string filename);
        void calculateTorqueLine();
        float getMaxTorque();
};

#endif // ENGINE_H_INCLUDED
