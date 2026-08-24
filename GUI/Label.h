#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED


#include <string>

#include "GUIObject.h"

#include "../Scripting/Utils/LuaMacros.h"

#include "../Utils/RFont.h"


class Label : public GUIObject
{
    private:
        RFont* _font;

        std::string _text;
        std::vector<unsigned int> _unicodeText;

        unsigned int _width;
        unsigned int _height;

        unsigned int _maxWidth;
        unsigned int _maxHeight;

        std::vector<glm::vec3>  _charsOffsets;
        std::vector<glm::mat4>  _charsTransforms;
        std::vector<bool>       _charsTransformsIs;

        void decodeTextToUnicode();
        bool buildTextData();
        void clearTextData();

        void calculateVerticesTransformMatrix(unsigned int index);

        inline virtual void changedVerticesTransformMatrixParameters()
        {
            for (std::vector<bool>::iterator i = _charsTransformsIs.begin(); i != _charsTransformsIs.end(); ++i)
            {
                *i = false;
            }
        }

    public:
        Label(RFont* font = NULL);
        ~Label();

        LUAF void setFont(RFont* font);
        LUAF void setText(std::string text);
        LUAF void setMaxWidth(unsigned int maxWidth);
        LUAF void setMaxHeight(unsigned int maxHeight);

        LUAF RFont* getFont();
        LUAF std::string getText();
        LUAF unsigned int getWidth();
        LUAF unsigned int getHeight();
        LUAF unsigned int getMaxWidth();
        LUAF unsigned int getMaxHeight();

        virtual glm::mat4& getVerticesTransformMatrix(unsigned int vboIndex = 0);

        virtual void addDataToRenderList(GUIRenderList* renderList);

};


#endif // LABEL_H_INCLUDED
