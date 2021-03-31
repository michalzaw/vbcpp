#include "Desktop.h"

#include "Bus.h"

#include "../Game/GameEnvironment.h"

#include "../Utils/Helpers.hpp"


DesktopIndicatorType getDesktopIndicatorTypeFromString(std::string name)
{
    for (int i = 0; i < INDICATORS_COUNT; ++i)
    {
        if (desktopIndicatorTypeStrings[i] == name)
            return static_cast<DesktopIndicatorType>(i);
    }
}


DesktopButtonAction getDesktopButtonActionFromString(std::string name)
{
    for (int i = 0; i < BUTTON_ACTIONS_COUNT; ++i)
    {
        if (desktopButtonActionStrings[i] == name)
            return static_cast<DesktopButtonAction>(i);
    }
}


DesktopLightType getDesktopLightTypeFromString(std::string name)
{
    for (int i = 0; i < DESKTOP_LIGHTS_COUNT; ++i)
    {
        if (desktopLightTypeStrings[i] == name)
            return static_cast<DesktopLightType>(i);
    }
}


Desktop::Desktop(RenderObject* desktopRenderObject, ClickableObject* desktopClickableObject)
    : _desktopRenderObject(desktopRenderObject), _desktopClickableObject(desktopClickableObject),
    _desktopSceneObject(NULL)
{
    if (_desktopRenderObject != NULL)
    {
        _desktopSceneObject = _desktopRenderObject->getSceneObject();
    }
}


void Desktop::catchInput()
{
    if (_desktopClickableObject != NULL && _desktopClickableObject->isClicked())
    {
        for (ModelNode* modelNode : _desktopClickableObject->getClickedNodes())
        {
            for (int i = 0; i < _buttons.size(); ++i)
            {
                if (modelNode == _buttons[i].modelNode)
                {
                    clickButton(i);
                }
            }
        }
    }
}


void Desktop::addIndicator(const std::string& indicatorNodeNameInModel, const std::string& variable, float maxAngle, float maxValue, float minValue, float minAngle, glm::vec3 rotationAxis)
{
    ModelNode* modelNode = _desktopRenderObject->getModelNodeByName(indicatorNodeNameInModel);

    Indicator newIndicator;
    newIndicator.modelNode = modelNode;
    newIndicator.variableName = variable;
    newIndicator.rotationAxis = rotationAxis;

    newIndicator.rotationCurve.push_back(glm::vec2(minValue, minAngle));
    newIndicator.rotationCurve.push_back(glm::vec2(maxValue, maxAngle));

    _indicators.push_back(newIndicator);
}


void Desktop::addIndicator(const std::string& indicatorNodeNameInModel, const std::string& variable, const std::vector<glm::vec2> rotationCurve, glm::vec3 rotationAxis)
{
    ModelNode* modelNode = _desktopRenderObject->getModelNodeByName(indicatorNodeNameInModel);

    Indicator newIndicator;
    newIndicator.modelNode = modelNode;
    newIndicator.variableName = variable;
    newIndicator.rotationCurve = rotationCurve;
    newIndicator.rotationAxis = rotationAxis;

    _indicators.push_back(newIndicator);
}


void Desktop::addButton(std::string buttonNodeNameInModel, glm::vec3 rotationAxix, std::vector<DesktopButtonState> states, bool isReturning)
{
    ModelNode* modelNode = _desktopRenderObject->getModelNodeByName(buttonNodeNameInModel);

    DesktopButton button;
    button.setData(modelNode, rotationAxix, states, isReturning);

    _buttons.push_back(button);
}


DesktopButton& Desktop::getButton(int index)
{
    return _buttons[index];
}


void Desktop::setLight(DesktopLightType type, std::string lightNodeNameInModel, glm::vec3 desktopBacklightingColor, glm::vec3 lightColor)
{
    ModelNode* modelNode = _desktopRenderObject->getModelNodeByName(lightNodeNameInModel);

    _lights[type].modelNode = modelNode;
    _lights[type].desktopBacklightingColor = desktopBacklightingColor;
    _lights[type].lightColor = lightColor;
}


DesktopLight& Desktop::getDesktopLight(DesktopLightType type)
{
    return _lights[type];
}


void Desktop::setButtonState(int index, unsigned int state)
{
    DesktopButton& button = getButton(index);
    if (button.modelNode == NULL || state >= button.statesCount)
        return;

    button.currentState = state;

    //button.modelNode->getTransform().setPosition(button.translateForStates[state]);
    //button.modelNode->getTransform().setRotation(button.rotateForStates[state]);
}


void Desktop::clickButton(int index)
{
    DesktopButton& button = _buttons[index];
	if (button.modelNode == NULL)
		return;

    button.currentState = (button.currentState + 1) % button.statesCount;


    if (button.states[button.currentState].action)
    {
        button.states[button.currentState].action();
    }

    if (button.states[button.currentState].actionWithParamInt)
    {
        button.states[button.currentState].actionWithParamInt(button.states[button.currentState].actionParam);
    }
}


void Desktop::setLightBacklightingState(DesktopLightType type, bool isEnable)
{
    DesktopLight& light = getDesktopLight(type);
    if (light.isBacklightEnable == isEnable || light.modelNode == NULL)
        return;

    light.isBacklightEnable = isEnable;

    glm::vec3 color = light.desktopBacklightingColor;
    if (!isEnable)
        color = -color;

    for (int i = 0; i < light.modelNode->getMeshesCount(); ++i)
    {
        _desktopRenderObject->getMaterial(light.modelNode->getMesh(i)->materialIndex)->emissiveColor += glm::vec4(color.r, color.g, color.b, 0.0f);
    }
}


void Desktop::setLightState(DesktopLightType type, bool isEnable)
{
    DesktopLight& light = getDesktopLight(type);
    if (light.isLightEnable == isEnable || light.modelNode == NULL)
        return;

    light.isLightEnable = isEnable;

    glm::vec3 color = light.lightColor;
    if (!isEnable)
        color = -color;

    for (int i = 0; i < light.modelNode->getMeshesCount(); ++i)
    {
        _desktopRenderObject->getMaterial(light.modelNode->getMesh(i)->materialIndex)->emissiveColor += glm::vec4(color.r, color.g, color.b, 0.0f);
    }
}


void Desktop::setDesktopBacklightingState(bool isEnable)
{
    for (int i = 0; i < DESKTOP_LIGHTS_COUNT; ++i)
    {
        setLightBacklightingState(static_cast<DesktopLightType>(i), isEnable);
    }
}


void Desktop::update(float deltaTime)
{
    catchInput();

    for (int i = 0; i < _buttons.size(); ++i)
    {
        if (_buttons[i].modelNode != NULL)
        {
            glm::vec3 destPos = _buttons[i].states[_buttons[i].currentState].translation;
            glm::vec3 curPos = _buttons[i].modelNode->getTransform().getPosition();

            float destRot = _buttons[i].states[_buttons[i].currentState].rotation;
            float curRot = _buttons[i].currentRotation;

            if (destPos.x == curPos.x && destPos.y == curPos.y && destPos.z == curPos.z &&
                destRot == curRot)
            {
                if (_buttons[i].isReturning && _buttons[i].currentState == 1)
                    _buttons[i].currentState = 0;
            }

            if (destPos.x != curPos.x || destPos.y != curPos.y || destPos.z != curPos.z)
            {
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

            // rotation
            if (destRot != curRot)
            {
                float deltaRotation = (destRot - curRot) / abs(destRot - curRot);
                _buttons[i].currentRotation += deltaRotation * 1.25f * deltaTime;

                if (deltaRotation > 0 && _buttons[i].currentRotation > destRot || deltaRotation < 0 && _buttons[i].currentRotation < destRot)
                {
                    _buttons[i].currentRotation = destRot;
                }

                glm::quat rotation = glm::angleAxis(_buttons[i].currentRotation, _buttons[i].rotationAxis);
                _buttons[i].modelNode->getTransform().setRotationQuaternion(rotation);
            }
        }
    }

    for (int i = 0; i < _indicators.size(); ++i)
    {
        Indicator& indicator = _indicators[i];
        float variableValue = GameEnvironment::Variables::floatVaribles[indicator.variableName];

        indicator.valueFromVariable = -getValueFromCurveInPoint(indicator.rotationCurve, variableValue);

        if (_indicators[i].modelNode != NULL)
        {
            float destRot = _indicators[i].valueFromVariable;
            float curRot = _indicators[i].currentValue;
            if (destRot == curRot)
            {
                continue;
            }

            float deltaRotation = (destRot - curRot) * 4.0f * deltaTime;
            _indicators[i].currentValue += deltaRotation;

            glm::quat rotation = glm::angleAxis(_indicators[i].currentValue, _indicators[i].rotationAxis);
            _indicators[i].modelNode->getTransform().setRotationQuaternion(rotation);
        }
    }
}
