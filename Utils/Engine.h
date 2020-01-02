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


enum EngineState
{
	ES_OFF,
	ES_STARTING,
	ES_RUN,
	ES_STOPPING
};


class Engine
{
    public:
        Engine(const std::string filename);
        virtual ~Engine();

		void setState(EngineState state);

        float getThrottle() { return _throttle; }

        float getCurrentRPM() { return _currentRPM; }

        void setRPM(float wheelAngularVelocity, float gearRatio);

        bool isRunning() { return _state == ES_RUN; }

		EngineState getState() { return _state; }

        void setThrottle(float throttle) { _throttle = throttle; }

        void throttleUp();

        void throttleDown();

        float getCurrentTorque() { return _currentTorque; }

		float getDifferentialRatio() { return _differentialRatio; }

        std::string getSoundFilename() { return _soundFilename; }

		float getSoundRpm() { return _soundRpm; }

        float getSoundVolume() { return _volume; }

		std::string getStartSoundFilename() { return _startSoundFilename; }

		std::string getStopSoundFilename() { return _stopSoundFilename; }

        void update(float dt);


    protected:
        EngineState _state;

        float   _throttle;
        float   _currentRPM;
        float   _currentTorque;
        float   _maxRPM;

		float _differentialRatio;

        std::string _soundFilename;
		float		_soundRpm;
        float       _volume;
		std::string _startSoundFilename;
		std::string _stopSoundFilename;

        std::vector<point>  _torqueCurve;
        std::vector<params> _curveParams;

        void loadData(const std::string filename);
        void calculateTorqueLine();
        float getMaxTorque();
};

#endif // ENGINE_H_INCLUDED
