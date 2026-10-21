#ifndef GEARBOX_H_INCLUDED
#define GEARBOX_H_INCLUDED



#include <vector>
#include <string>

#include "../Scripting/Utils/LuaMacros.h"


enum Mode
{
    EGM_REVERSE = 0,
    EGM_NEUTRAL,
    EGM_FORWARD
};


class Gearbox
{
    public:
        Gearbox(const std::string filename);
        virtual ~Gearbox();

        LUAF const int currentGear() { return _currentGear; }
        LUAF const float currentRatio() { return _gears[_currentGear]; }

        LUAF void shiftUp();
        LUAF void shiftDown();
        LUAF void setNeutral();

    protected:
        std::vector<float>  _gears;
        int                _gearCount;
        int                _currentGear;

        void loadData(const std::string filename);
};

#endif // GEARBOX_H_INCLUDED
