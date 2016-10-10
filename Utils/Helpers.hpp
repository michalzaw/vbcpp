#ifndef HELPERS_HPP_INCLUDED
#define HELPERS_HPP_INCLUDED

#include <algorithm>

#include <glm/glm.hpp>
#include <cstdlib>
#include <sstream>
#include <iostream>

template<typename T> void safe_delete(T*& a) {
  delete a;
  a = 0;
}

template <typename T>
T clamp(const T& what, const T& a, const T& b)
{
       return std::min(b, std::max(what, a));
}


#endif // HELPERS_HPP_INCLUDED
