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


DesktopButtonType getDesktopButtonTypeFromString(std::string name)
{
    for (int i = 0; i < BUTTONS_COUNT; ++i)
    {
        if (desktopButtonTypeStrings[i] == name)
            return static_cast<DesktopButtonType>(i);
    }
}


Desktop::Desktop(RenderObject* desktopRenderObject)
    : _desktopRenderObject(desktopRenderObject),
    _desktopSceneObject(NULL)
{
    if (_desktopRenderObject != NULL)
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


void Desktop::setButton(DesktopButtonType type, std::string buttonNodeNameInModel, std::vector<glm::vec3>& translateForStates, std::vector<glm::vec3>& rotateForStates, bool isReturning)
{
    ModelNode* modelNode = _desktopRenderObject->getModelNodeByName(buttonNodeNameInModel);

    _buttons[type].setData(modelNode, translateForStates, rotateForStates, isReturning);
}


DesktopButton& Desktop::getButton(DesktopButtonType type)
{
    return _buttons[type];
}


void Desktop::setIndicatorValue(DesktopIndicatorType type, float value)
{
    Indicator& indicator = getIndicator(type);
    if (indicator.modelNode == NULL)
        return;

    float v = clamp(value, indicator.minValue, indicator.maxValue) - indicator.minValue;

    indicator.modelNode->getTransform().setRotation(0, -v / (indicator.maxValue - indicator.minValue) * indicator.maxAngle, 0);
}


void Desktop::setButtonState(DesktopButtonType type, unsigned int state)
{
    DesktopButton& button = getButton(type);
    if (button.modelNode == NULL || state >= button.statesCount)
        return;

    button.currentState = state;

    //button.modelNode->getTransform().setPosition(button.translateForStates[state]);
    //button.modelNode->getTransform().setRotation(button.rotateForStates[state]);
}


void Desktop::update(float deltaTime)
{
    for (int i = 0; i < BUTTONS_COUNT; ++i)
    {
        if (_buttons[i].modelNode != NULL)
        {
            glm::vec3 destPos = _buttons[i].translateForStates[_buttons[i].currentState];
            glm::vec3 curPos = _buttons[i].modelNode->getTransform().getPosition();
            if (destPos.x == curPos.x && destPos.y == curPos.y && destPos.z == curPos.z)
            {
                if (_buttons[i].isReturning && _buttons[i].currentState == 1)
                    _buttons[i].currentState = 0;
                continue;
            }
            glm::vec3 direction = glm::normalize(destPos - curPos);
            glm::vec3 deltaPos = direction * deltaTime * 0.025;

            _buttons[i].modelNode->getTransform().move(deltaPos);

            glm::vec3 pos = _buttons[i].modelNode->getTransform().getPosition();
            if (direction.x > 0 && pos.x > destPos.x || direction.x < 0 && pos.x < destPos.x)
                pos.x = destPos.x;
            if (direction.y > 0 && pos.y > destPos.y || direction.y < 0 && pos.y < destPos.y)
                pos.y = destPos.y;
            if (direction.z > 0 && pos.z > destPos.z || direction.z < 0 && pos.z < destPos.z)
                pos.z = destPos.z;

            _buttons[i].modelNode->getTransform().setPosition(pos);

        }
    }
}
