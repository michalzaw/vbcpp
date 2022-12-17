#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include <string>
#include <vector>

#include <glm/glm.hpp>

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


enum AudibilityType
{
    AT_ALWAYS,
    AT_INTERIOR,
    AT_EXTERIOR,

    AT_TYPE_COUNT
};


const std::string audibilityTypeStrings[] = { "always", "interior", "exterior" };
AudibilityType getAudibilityTypeFromStrings(const std::string& name);


enum SoundVolumeCurveVariable
{
    SVCV_THROTTLE,
    SVCV_RPM,

    SVCV_VARIABLE_COUNT
};


const std::string soundVolumeCurveStrings[] = { "throttle", "rpm" };
SoundVolumeCurveVariable getSoundVolumeCurveFromStrings(const std::string& name);


enum SoundTrigger
{
    ST_START_ENGINE,
    ST_STOP_ENGINE,

    ST_TRIGGERS_COUNT
};


const std::string soundTriggerStrings[] = { "start_engine", "stop_engine" };
SoundTrigger getSoundTriggerFromStrings(const std::string& name);


struct SoundVolumeCurve
{
    SoundVolumeCurveVariable variable;
    std::vector<glm::vec2> points;
};


struct SoundDefinition
{
    std::string soundFilename;
    AudibilityType audibilityType;
    float rpm;
    bool looped;
    float volume;
    float playDistance;
    SoundTrigger trigger;

    std::vector<SoundVolumeCurve> volumeCurves;
};


namespace tinyxml2
{
    class XMLElement;
}


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

        void throttleUp(float dt);

        void throttleDown(float dt);

        float getCurrentTorque() { return _currentTorque; }

		float getDifferentialRatio() { return _differentialRatio; }

        const std::vector<SoundDefinition>& getEngineLoopedSounds() { return _engineLoopedSounds; }

        const std::vector<SoundDefinition>& getEngineSounds() { return _engineSounds; }

        void update(float dt);

    protected:
        EngineState _state;

        float   _throttle;
        float   _currentRPM;
        float   _currentTorque;
        float   _maxRPM;

		float _differentialRatio;

        std::vector<SoundDefinition> _engineLoopedSounds;
        std::vector<SoundDefinition> _engineSounds;

        std::vector<point>  _torqueCurve;
        std::vector<params> _curveParams;

        void loadData(const std::string filename);
        void calculateTorqueLine();
        float getMaxTorque();

        void loadSounds(tinyxml2::XMLElement* soundsElement);
        void loadVolumeCurvesForSounds(tinyxml2::XMLElement* soundElement, SoundDefinition& soundDefinition);
};

#endif // ENGINE_H_INCLUDED
