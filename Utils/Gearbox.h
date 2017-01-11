#ifndef GEARBOX_H_INCLUDED
#define GEARBOX_H_INCLUDED



#include <vector>
#include <string>

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

        const int currentGear() { return _currentGear; }
        const float currentRatio() { return _gears[_currentGear]; }

        void shiftUp();
        void shiftDown();
        void setNeutral();

    protected:
        std::vector<float>  _gears;
        int                _gearCount;
        int                _currentGear;

        void loadData(const std::string filename);
};

#endif // GEARBOX_H_INCLUDED
