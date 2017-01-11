#include "Engine.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include <iostream>


Engine::Engine(std::string filename)
: _isRunning(false),
_throttle(0.0f), _currentRPM(0.0f), _currentTorque(0.0f), _maxRPM(0.0f)
{
    loadData(filename);

    calculateTorqueLine();
}


Engine::~Engine()
{
    _torqueCurve.clear();
}


void Engine::turnOn()
{
    _isRunning = true;

    _currentRPM = _torqueCurve[0].rpm;
}


void Engine::turnOff()
{
    _isRunning = false;

    _currentRPM = 0.0f;
}


void Engine::loadData(std::string filename)
{
    std::string fullPath = "Parts/" + filename + ".xml";

    XMLDocument doc;
    doc.LoadFile( fullPath.c_str() );

    // Search for main element - Engine
    XMLElement* engElement = doc.FirstChildElement("Engine");
    if (engElement == nullptr) std::cout << "Engine element not found! Is it correct engine file?" << std::endl;

    XMLElement* engDesc = engElement->FirstChildElement("Description");
    if (engDesc == nullptr) std::cout << "Description element not found" << std::endl;

    // Load file description
    std::string author(engDesc->Attribute("Author"));
    std::string model(engDesc->Attribute("Model"));
    std::string comment(engDesc->Attribute("Comment"));

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

    for (int i = 0; i < _torqueCurve.size(); i++)
        std::cout << "Point " << i << ": " << _torqueCurve[i].rpm << " - " << _torqueCurve[i].torque << std::endl;
}


void Engine::calculateTorqueLine()
{
    int pointCount = _torqueCurve.size();
    _curveParams.resize(pointCount);

	for ( char i = 0; i < pointCount; i++ )
	{
		_curveParams[i].a = ( static_cast<float>(_torqueCurve[i+1].torque) - static_cast<float>(_torqueCurve[i].torque) ) / ( static_cast<float>(_torqueCurve[i+1].rpm) - static_cast<float>(_torqueCurve[i].rpm) );
		printf("Torque a parameter: %3.2f\n", _curveParams[i].a );
		_curveParams[i].b = (float)_torqueCurve[i].torque - ( _curveParams[i].a * (float)_torqueCurve[i].rpm );
		printf("Torque b parameter: %3.2f\n", _curveParams[i].b );
	}
}


void Engine::update()
{
    if (!_isRunning)
        _currentTorque = 0.0f;
    else
    if (_throttle > 0)
    {
        if ( _currentRPM < _maxRPM )
        {
            _currentRPM += _throttle;
        }
        else
            _currentRPM = _maxRPM;
    }
    else
    {
        if (_currentRPM > _torqueCurve[0].rpm)
            _currentRPM -= 1;
        else
            _currentRPM = _torqueCurve[0].rpm;
    }

    _currentTorque = getMaxTorque();
}


float Engine::getMaxTorque()
{
    float maxTorque;

	if ( _currentRPM < _torqueCurve[0].rpm )
        maxTorque = 0.0f;
	else
	{
		for ( char i = 0; i < _torqueCurve.size() - 1; i++ )
		{
			if ( (_currentRPM >= _torqueCurve[i].rpm) && (_currentRPM <= _torqueCurve[i+1].rpm) )
				maxTorque = (_curveParams[i].a * _currentRPM + _curveParams[i].b);
		}
	}

	return maxTorque; // Value must be decreased since it's to big
}
