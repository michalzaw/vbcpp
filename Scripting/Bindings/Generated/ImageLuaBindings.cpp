// Generated file
#include "ImageLuaBindings.h"

#include "../../../GUI/Image.h"


namespace ImageLuaBindings
{
    void bind(sol::state* lua)
    {
        lua->new_usertype<Image>("Image",
            sol::base_classes, sol::bases<GUIObject>(),

            "setTexture", &Image::setTexture,
            "setTextureRect", &Image::setTextureRect,
            "getTexture", &Image::getTexture,
            "getTextureRect", &Image::getTextureRect,
            "getSize", &Image::getSize,
            "getRealSize", &Image::getRealSize,
            "setInvertX", &Image::setInvertX,
            "setInvertY", &Image::setInvertY,
            "getInvertX", &Image::getInvertX,
            "getInvertY", &Image::getInvertY



        );
    }
}
