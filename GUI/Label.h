#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED


#include <string>

#include "GUIObject.h"

#include "../Utils/RFont.h"


class Label : public GUIObject
{
    private:
        RFont* _font;

        std::string _text;
        unsigned int _textSize;

        unsigned int _maxWidth;
        unsigned int _maxHeight;

        std::vector<glm::vec3>  _charsOffsets;
        std::vector<glm::mat4>  _charsTransforms;
        std::vector<bool>       _charsTransformsIs;

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

        void setFont(RFont* font);
        void setText(std::string text);
        void setMaxWidth(unsigned int maxWidth);
        void setMaxHeight(unsigned int maxHeight);

        RFont* getFont();
        std::string getText();
        unsigned int getMaxWidth();
        unsigned int getMaxHeight();

        virtual glm::mat4& getVerticesTransformMatrix(unsigned int vboIndex = 0);

        virtual void addDataToRenderList(GUIRenderList* renderList);

};


#endif // LABEL_H_INCLUDED
