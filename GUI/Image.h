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

        glm::vec2   _size;                      // Wykorzystywane wewnetrznie, zalezy np od rozmiaru tekstury

        glm::mat4 _verticesTransformMatrix;
        glm::mat4 _texCoordTransformMatrix;

        bool _verticesTransformMatrixIs;
        bool _texCoordTransformMatrixIs;


        bool _isInitialized;


        void createVBO();

        void setSize(glm::vec2 size);

        void calculateVerticesTransformMatrix();
        void calculateTexCoordTransformMatrix();

        inline virtual void changedVerticesTransformMatrixParameters() { _verticesTransformMatrixIs = false; }
        inline virtual void changedTexCoordTransformMatrixParameters() { _texCoordTransformMatrixIs = false; }

    public:
        Image(RTexture* texture);
        Image(RTexture* texture, const UintRect& textureRect);
        ~Image();

        void setTexture(RTexture* texture, bool isResetRect = true);
        void setTextureRect(const UintRect& rect);

        RTexture* getTexture();
        UintRect& getTextureRect();

        glm::vec2 getSize();

        virtual glm::mat4& getVerticesTransformMatrix(unsigned int vboIndex = 0);
        virtual glm::mat4& getTexCoordTransformMatrix(unsigned int vboIndex = 0);

        virtual void addDataToRenderList(GUIRenderList* renderList);

};


#endif // IMAGE_H_INCLUDED
