#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/detail/type_vec2.hpp>

#include "GUIObject.h"

#include "../Utils/RTexture.h"


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


class Image : public GUIObject
{
    protected:
        RTexture* _texture;

        UintRect _textureRect;


        virtual void calculateTexCoordTransformMatrix();

    public:
        Image(RTexture* texture);
        Image(RTexture* texture, const UintRect& textureRect);
        ~Image();

        void setTexture(RTexture* texture, bool isResetRect = true);
        void setTextureRect(const UintRect& rect);

        RTexture* getTexture();
        UintRect& getTextureRect();

};


#endif // IMAGE_H_INCLUDED
