#include "Helpers.hpp"

#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

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

