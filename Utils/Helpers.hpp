#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED

#include <algorithm>

#include <glm/glm.hpp>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <vector>

#include <btBulletDynamicsCommon.h>

template<typename T> void safe_delete(T*& a) {
  delete a;
  a = 0;
}

template <typename T>
T clamp(const T& what, const T& a, const T& b)
{
       return std::min(b, std::max(what, a));
}

glm::vec2 XMLstringToVec2(const char* xmlstring);
glm::vec3 XMLstringToVec3(const char* xmlstring);
//<<<<<<< HEAD
//glm::vec2 XMLstringToVec2(const char* xmlstring);
//=======
glm::vec4 XMLstringToVec4(const char* xmlstring);
//>>>>>>> mat_xml_serv

btVector3 XMLstringToBtVec3(const char* xmlstring);

inline int next_p2(int n) { int res = 1; while (res < n) res <<= 1; return res; }

template <typename T>
inline bool isVectorContains(std::vector<T>& vector, T& element)
{
    return std::find(vector.begin(), vector.end(), element) != vector.end();
}


#endif // HELPERS_HPP_INCLUDED
