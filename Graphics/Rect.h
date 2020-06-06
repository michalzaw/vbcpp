#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/detail/type_vec2.hpp>


template <typename TYPE>
struct Rect
{
    glm::tvec2<TYPE> position;      // Left, top corner
    glm::tvec2<TYPE> size;

    Rect()
        : position(TYPE(), TYPE()), size(TYPE(), TYPE()) {}

    Rect(glm::tvec2<TYPE> pos, glm::tvec2<TYPE> size)
        : position(pos), size(size) {}

    Rect(TYPE left, TYPE bottom, TYPE width, TYPE height)
        : position(left, bottom), size(width, height) {}

};


typedef Rect<unsigned int> UintRect;


#endif // RECT_H_INCLUDED
