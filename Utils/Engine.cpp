#include "Engine.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include <iostream>

#include "Math.h"
#include "FilesHelper.h"
#include "ResourceManager.h"
#include "Logger.h"
#include "XmlUtils.h"

#include "../Game/Directories.h"


Engine::Engine(std::string filename)
: _state(ES_OFF),
_throttle(0.0f), _currentRPM(0.0f), _currentTorque(0.0f), _maxRPM(0.0f), _differentialRatio(3.45f)
{
    loadData(filename);

    calculateTorqueLine();
}


Engine::~Engine()
{
    _torqueCurve.clear();
}


void Engine::setState(EngineState state)
{
    _state = state;

	if (_state == ES_RUN)
	{
		_currentRPM = _torqueCurve[0].rpm;
	}
	else if (_state == ES_OFF || _state == ES_STOPPING)
	{
		_currentRPM = 0.0f;
	}
}


void Engine::setRPM(float wheelAngularVelocity, float gearRatio)
{
    if (isRunning())
    {
        float rpm = (wheelAngularVelocity * abs(gearRatio) * _differentialRatio * 60.0f) / PI;
        //if (_currentRPM < _torqueCurve[0].rpm)
        //    _currentRPM = static_cast<float>(_torqueCurve[0].rpm);
        if (rpm > _torqueCurve[0].rpm)
            _currentRPM = rpm;
        else if (rpm < _torqueCurve[0].rpm)
            _currentRPM = _torqueCurve[0].rpm;
        //else if (rpm > _maxRPM)
        //    _currentRPM = _maxRPM;

        //printf("RPM: %.4f\n", rpm);
    }
}


void Engine::throttleUp()
{
    if (_throttle < 1.0f)
        _throttle += 0.001f;
    else
        _throttle = 1.0f;
}


void Engine::throttleDown()
{
    if (_throttle > 0)
        _throttle -= 0.002f;
    else
        _throttle = 0;
}


void Engine::loadData(std::string filename)
{
    std::string fullPath = GameDirectories::BUS_PARTS + filename + ".xml";

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(fullPath))
		fullPath = ResourceManager::getInstance().getAlternativeResourcePath() + fullPath;
#endif // DEVELOPMENT_RESOURCES

    XMLDocument doc;
    doc.LoadFile(fullPath.c_str());

    // Search for main element - Engine
    XMLElement* engElement = doc.FirstChildElement("Engine");
	if (engElement == nullptr)
	{
		Logger::error("Engine element not found!Is it correct engine file ? ");
		return;
	}

    XMLElement* engDesc = engElement->FirstChildElement("Description");
	if (engDesc == nullptr)
		Logger::warning("Description element not found");

    // Load file description
    std::string author(engDesc->Attribute("author"));
    std::string model(engDesc->Attribute("name"));
    std::string comment(engDesc->Attribute("comment"));

    XMLElement* engSound = engElement->FirstChildElement("Sound");

	_soundFilename = GameDirectories::BUS_PARTS + std::string(engSound->Attribute("file"));

	_soundRpm = XmlUtils::getAttributeFloatOptional(engSound, "rpm", 1000.0f);
    _volume = XmlUtils::getAttributeFloatOptional(engSound, "volume", 1.0f);

	_startSoundFilename = GameDirectories::BUS_PARTS + XmlUtils::getAttributeStringOptional(engSound, "startFile");
	_stopSoundFilename = GameDirectories::BUS_PARTS + XmlUtils::getAttributeStringOptional(engSound, "stopFile");

    std::cout << "Engine sound: " << _soundFilename << std::endl;
    std::cout << "Engine volume: " << _volume << std::endl;

    XMLElement* pointList = engElement->FirstChildElement("Points");

    int curvePoints = atoi(pointList->Attribute("count"));
    std::cout << "Point count: " << curvePoints << std::endl;

    XMLElement* curvePoint = pointList->FirstChildElement("Point");

    while (curvePoint != nullptr)
    {
        point p;
        p.rpm = atof(curvePoint->Attribute("rpm"));
        p.torque = atof(curvePoint->Attribute("torque"));
        _torqueCurve.push_back(p);

        _maxRPM = p.rpm; // set maxRPM in every iteration; the last value will be maximum

        curvePoint = curvePoint->NextSiblingElement("Point");
    }

    std::cout << "*** ENGINE DATA ***" << std::endl;
    std::cout << "Author: " << author << std::endl;
    std::cout << "Model: " << model << std::endl;
    std::cout << "Comment: " << comment << std::endl;

    std::cout << "Point count: " << _torqueCurve.size() << std::endl;
}


void Engine::calculateTorqueLine()
{
    int pointCount = _torqueCurve.size();
    _curveParams.resize(pointCount);

	// TODO: verify
	//for ( char i = 0; i < pointCount; i++ )
	for (char i = 0; i < pointCount - 1; i++)
	{
		_curveParams[i].a = ( static_cast<float>(_torqueCurve[i+1].torque) - static_cast<float>(_torqueCurve[i].torque) ) / ( static_cast<float>(_torqueCurve[i+1].rpm) - static_cast<float>(_torqueCurve[i].rpm) );
		//printf("Torque a parameter: %3.2f\n", _curveParams[i].a );
		_curveParams[i].b = (float)_torqueCurve[i+1].torque - ( _curveParams[i].a * (float)_torqueCurve[i+1].rpm );
		//printf("Torque b parameter: %3.2f\n", _curveParams[i].b );
	}
}


void Engine::update(float dt)
{
    if (isRunning())
    {
        if (_throttle > 0)
        {
			// wykomentowanie tych dwoch lini nie ma wplywu na zachowanie silnika, tak jak by byly nie potrzebne
            if (_currentRPM < _maxRPM)
                _currentRPM += 1000 * _throttle * dt;
            //else
            //    _currentRPM = _maxRPM;
        }
        else
        {
            if (_currentRPM > _torqueCurve[0].rpm)
                _currentRPM -= 1000 * dt;
           // else
            //    _currentRPM = _torqueCurve[0].rpm;
        }
    }


    //_currentRPM = (wheelAngularVelocity * abs(gearRatio) * 5.45f * 60.0f) / 3.14f;

    _currentTorque = getMaxTorque() * _throttle;
}


float Engine::getMaxTorque()
{
    float maxTorque = 0.0f;

	if ( _currentRPM < _torqueCurve[0].rpm )
        maxTorque = 0.0f;
	else
	{
		for ( unsigned char i = 0; i < _torqueCurve.size() - 1; i++ )
		{
			if ( (_currentRPM >= _torqueCurve[i].rpm) && (_currentRPM <= _torqueCurve[i+1].rpm) )
				maxTorque = (_curveParams[i].a * _currentRPM + _curveParams[i].b);
		}
	}

	if (_currentRPM > _maxRPM)
	{
		maxTorque = 0;
	}

	return maxTorque; // Value must be decreased since it's to big
}
