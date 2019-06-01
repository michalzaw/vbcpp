#ifndef DESKTOP_H_INCLUDED
#define DESKTOP_H_INCLUDED


#include <../Graphics/RenderObject.h>
#include <../Graphics/ModelNode.h>

#include <../Scene/SceneObject.h>


enum DesktopIndicatorType
{
    DIT_SPEEDOMETER,
    DIT_TACHOMER,

    INDICATORS_COUNT
};


enum DesktopButtonTypes
{
    DBT_DOOR_1,
    DBT_DOOR_2,

    BUTTONS_COUNT
};


struct Indicator
{
    ModelNode* modelNode;
    float maxValue;
    float maxAngle;
    float minValue;

    Indicator()
    {
        modelNode = NULL;
    }
};


class Desktop
{
    private:
        SceneObject* _desktopSceneObject;
        RenderObject* _desktopRenderObject;

        Indicator _indicators[INDICATORS_COUNT];

    public:
        Desktop(RenderObject* desktopRenderObject);

        void setIndicator(DesktopIndicatorType type, std::string indicatorNodeNameInModel, float maxAngle, float maxValue, float minValue = 0.0f);
        Indicator& getIndicator(DesktopIndicatorType type);

        void setIndicatorValue(DesktopIndicatorType type, float value);

};


#endif // DESKTOP_H_INCLUDED
