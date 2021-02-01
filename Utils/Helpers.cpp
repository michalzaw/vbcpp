#include "Helpers.hpp"

#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

#include "Math.h"

glm::vec2 XMLstringToVec2(const char* xmlstring)
{
    std::stringstream ss(xmlstring);
    //vec3 outVec = {0,0,0};

    std::string s1, s2;
    getline(ss, s1, ',');
    float n1 = (float)atof(s1.c_str());
    //std::cout << "N1 " << n1 << std::endl;

    getline(ss, s2, ',');
    float n2 = (float)atof(s2.c_str());
    //std::cout << "N2 " << n2 << std::endl;

    glm::vec2 outVec(n1, n2);
    //cout << "Sum: " << n1 + n2 + n3 << endl;
    return outVec;
}

glm::vec3 XMLstringToVec3(const char* xmlstring)
{
    std::stringstream ss(xmlstring);
    //vec3 outVec = {0,0,0};

    std::string s1, s2, s3;
    getline(ss, s1, ',');
    float n1 = (float)atof(s1.c_str());
    //std::cout << "N1 " << n1 << std::endl;

    getline(ss, s2, ',');
    float n2 = (float)atof(s2.c_str());
    //std::cout << "N2 " << n2 << std::endl;

    getline(ss, s3, ',');
    float n3 = (float)atof(s3.c_str());
    //std::cout << "N3 " << n3 << std::endl;

    glm::vec3 outVec(n1, n2, n3);
    //cout << "Sum: " << n1 + n2 + n3 << endl;
    return outVec;
}

glm::vec4 XMLstringToVec4(const char* xmlstring)
{
    std::stringstream ss(xmlstring);
    //vec3 outVec = {0,0,0};

    std::string s1, s2, s3, s4;
    getline(ss, s1, ',');
    float n1 = (float)atof(s1.c_str());
    //std::cout << "N1 " << n1 << std::endl;

    getline(ss, s2, ',');
    float n2 = (float)atof(s2.c_str());
    //std::cout << "N2 " << n2 << std::endl;

    getline(ss, s3, ',');
    float n3 = (float)atof(s3.c_str());
    //std::cout << "N3 " << n3 << std::endl;

    getline(ss, s4, ',');
    float n4 = (float)atof(s4.c_str());
    //std::cout << "N3 " << n3 << std::endl;

    glm::vec4 outVec(n1, n2, n3, n4);
    //cout << "Sum: " << n1 + n2 + n3 << endl;
    return outVec;
}

btVector3 XMLstringToBtVec3(const char* xmlstring)
{
    std::stringstream ss(xmlstring);
    //vec3 outVec = {0,0,0};

    std::string s1, s2, s3;
    getline(ss, s1, ',');
    float n1 = (float)atof(s1.c_str());

    getline(ss, s2, ',');
    float n2 = (float)atof(s2.c_str());

    getline(ss, s3, ',');
    float n3 = (float)atof(s3.c_str());

    btVector3 outVec(n1, n2, n3);

    return outVec;
}

float getValueFromCurveInPoint(const std::vector<glm::vec2>& curve, float point)
{
    const float p = clamp(point, curve[0].x, curve[curve.size() - 1].x);
    for (int i = 1; i < curve.size(); ++i)
    {
        if (p <= curve[i].x)
        {
            return lerp(curve[i - 1].y, curve[i].y, (p - curve[i - 1].x) / (curve[i].x - curve[i - 1].x));
        }
    }
}
