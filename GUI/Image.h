#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/detail/type_vec2.hpp>

#include "GUIObject.h"

#include "../Scripting/Utils/LuaMacros.h"

#include "../Graphics/Rect.h"
#include "../Utils/RTexture.h"


class Image : public GUIObject
{
    protected:
        RTexture* _texture;

        UintRect _textureRect;

        glm::vec2   _size;                      // Wykorzystywane wewnetrznie, zalezy np od rozmiaru tekstury

        bool _invertX;
        bool _invertY;

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

        LUAF void setTexture(RTexture* texture, bool isResetRect = true);
        LUAF void setTextureRect(const UintRect& rect);

        LUAF RTexture* getTexture();
        LUAF UintRect& getTextureRect();

        LUAF glm::vec2 getSize();
        LUAF glm::vec2 getRealSize();

        LUAF void setInvertX(bool invertX);
        LUAF void setInvertY(bool invertY);
        LUAF bool getInvertX();
        LUAF bool getInvertY();

        virtual glm::mat4& getVerticesTransformMatrix(unsigned int vboIndex = 0);
        virtual glm::mat4& getTexCoordTransformMatrix(unsigned int vboIndex = 0);

        virtual void addDataToRenderList(GUIRenderList* renderList);

};


#endif // IMAGE_H_INCLUDED
