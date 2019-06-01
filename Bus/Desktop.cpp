#include "Desktop.h"

#include "../Utils/Helpers.hpp"


DesktopIndicatorType getDesktopIndicatorTypeFromString(std::string name)
{
    for (int i = 0; i < INDICATORS_COUNT; ++i)
    {
        if (desktopIndicatorTypeStrings[i] == name)
            return static_cast<DesktopIndicatorType>(i);
    }
}


Desktop::Desktop(RenderObject* desktopRenderObject)
    : _desktopRenderObject(desktopRenderObject)
{
    _desktopSceneObject = _desktopRenderObject->getSceneObject();
}


void Desktop::setIndicator(DesktopIndicatorType type, std::string indicatorNodeNameInModel, float maxAngle, float maxValue, float minValue)
{
    ModelNode* modelNode = _desktopRenderObject->getModelNodeByName(indicatorNodeNameInModel);

    _indicators[type].modelNode = modelNode;
    _indicators[type].maxAngle = maxAngle;
    _indicators[type].maxValue = maxValue;
    _indicators[type].minValue = minValue;
}


Indicator& Desktop::getIndicator(DesktopIndicatorType type)
{
    return _indicators[type];
}


void Desktop::setIndicatorValue(DesktopIndicatorType type, float value)
{
    Indicator& indicator = getIndicator(type);
    if (indicator.modelNode == NULL)
        return;

    float v = clamp(value, indicator.minValue, indicator.maxValue) - indicator.minValue;

    indicator.modelNode->getTransform().setRotation(0, -v / (indicator.maxValue - indicator.minValue) * indicator.maxAngle, 0);
}
