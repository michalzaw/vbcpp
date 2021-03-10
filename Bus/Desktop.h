#ifndef DESKTOP_H_INCLUDED
#define DESKTOP_H_INCLUDED


#include "../Graphics/RenderObject.h"
#include "../Graphics/ModelNode.h"

#include "../Scene/SceneObject.h"
#include "../Scene/ClickableObject.h"


class Bus;


enum DesktopIndicatorType
{
    DIT_SPEEDOMETER,
    DIT_TACHOMETER,

    INDICATORS_COUNT
};


const std::string desktopIndicatorTypeStrings[] = { "speedometer", "tachometer" };
DesktopIndicatorType getDesktopIndicatorTypeFromString(std::string name);


enum DesktopButtonType
{
    DBT_DOOR_1,
    DBT_DOOR_2,
    DBT_DOOR_3,
    DBT_DOOR_4,

    BUTTONS_COUNT
};

const std::string desktopButtonTypeStrings[] = { "door1", "door2", "door3", "door4" };
DesktopButtonType getDesktopButtonTypeFromString(std::string name);


enum DesktopLightType
{
    DLT_DOOR_1,
    DLT_DOOR_2,
    DLT_DOOR_3,
    DLT_DOOR_4,
    DLT_BACKLIGHTING,

    DESKTOP_LIGHTS_COUNT
};

const std::string desktopLightTypeStrings[] = { "door1", "door2", "door3", "door4", "backlighting" };
DesktopLightType getDesktopLightTypeFromString(std::string name);


struct Indicator
{
    ModelNode* modelNode;
    std::string variableName;

    float maxValue;
    float maxAngle;
    float minValue;
    glm::vec3 rotationAxis;

    float valueFromVariable;
    float currentValue;

    Indicator()
    {
        modelNode = NULL;
    }
};


struct DesktopButton
{
    ModelNode* modelNode;
    unsigned int statesCount;
    glm::vec3 rotationAxis;
    std::vector<glm::vec3> translateForStates;
    std::vector<float> rotateForStates;
    bool isReturning;

    unsigned int currentState;

    float currentRotation;

    DesktopButton()
    {
        modelNode = NULL;
    }

    void setData(ModelNode* modelNode, glm::vec3 rotationAxis, std::vector<glm::vec3>& translateForStates, std::vector<float>& rotateForStates, bool isReturning)
    {
        this->modelNode = modelNode;
        this->statesCount = translateForStates.size();
        this->translateForStates = translateForStates;
        this->rotateForStates = rotateForStates;
        this->isReturning = isReturning;
        this->rotationAxis = rotationAxis;

        this->currentState = 0;
        this->currentRotation = 0.0f;
    }

};


struct DesktopLight
{
    ModelNode* modelNode;
    glm::vec3 desktopBacklightingColor;
    glm::vec3 lightColor;

    bool isBacklightEnable;
    bool isLightEnable;

    DesktopLight()
        : modelNode(NULL), isBacklightEnable(false), isLightEnable(false)
    {}

};


class Desktop
{
    private:
        SceneObject* _desktopSceneObject;
        RenderObject* _desktopRenderObject;

        std::vector<Indicator> _indicators;
        DesktopButton _buttons[BUTTONS_COUNT];
        DesktopLight _lights[DESKTOP_LIGHTS_COUNT];

    public:
        Desktop(RenderObject* desktopRenderObject);

        void addIndicator(const std::string& indicatorNodeNameInModel, const std::string& variable, float maxAngle, float maxValue, float minValue = 0.0f, glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f));

        void setButton(DesktopButtonType type, std::string buttonNodeNameInModel, glm::vec3 rotationAxix, std::vector<glm::vec3>& translateForStates, std::vector<float>& rotateForStates, bool isReturning);
        DesktopButton& getButton(DesktopButtonType type);

        void setLight(DesktopLightType type, std::string lightNodeNameInModel, glm::vec3 desktopBacklightingColor, glm::vec3 lightColor);
        DesktopLight& getDesktopLight(DesktopLightType type);

        void setIndicatorValue(DesktopIndicatorType type, float value);
        void setButtonState(DesktopButtonType type, unsigned int state);
        void clickButton(DesktopButtonType type);
        void setLightBacklightingState(DesktopLightType type, bool isEnable);
        void setLightState(DesktopLightType type, bool isEnable);
        void setDesktopBacklightingState(bool isEnable);

        void update(float deltaTime);

};


#endif // DESKTOP_H_INCLUDED
