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

    BUTTONS_COUNT
};

const std::string desktopButtonTypeStrings[] = { "door1", "door2" };
DesktopButtonType getDesktopButtonTypeFromString(std::string name);


enum DesktopLightType
{
    DLT_DOOR_1,
    DLT_DOOR_2,
    DLT_BACKLIGHTING,

    DESKTOP_LIGHTS_COUNT
};

const std::string desktopLightTypeStrings[] = { "door1", "door2", "backlighting" };
DesktopLightType getDesktopLightTypeFromString(std::string name);


struct Indicator
{
    ModelNode* modelNode;
    float maxValue;
    float maxAngle;
    float minValue;

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
    std::vector<glm::vec3> translateForStates;
    std::vector<glm::vec3> rotateForStates;
    bool isReturning;

    unsigned int currentState;

    DesktopButton()
    {
        modelNode = NULL;
    }

    void setData(ModelNode* modelNode, std::vector<glm::vec3>& translateForStates, std::vector<glm::vec3>& rotateForStates, bool isReturning)
    {
        this->modelNode = modelNode;
        this->statesCount = translateForStates.size() + 1;
        this->translateForStates = translateForStates;
        this->rotateForStates = rotateForStates;
        this->isReturning = isReturning;

        this->translateForStates.insert(this->translateForStates.begin(), glm::vec3(0.0f, 0.0f, 0.0f));
        this->rotateForStates.insert(this->rotateForStates.begin(), glm::vec3(0.0f, 0.0f, 0.0f));


        this->currentState = 0;
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

        Indicator _indicators[INDICATORS_COUNT];
        DesktopButton _buttons[BUTTONS_COUNT];
        DesktopLight _lights[DESKTOP_LIGHTS_COUNT];

    public:
        Desktop(RenderObject* desktopRenderObject);

        void setIndicator(DesktopIndicatorType type, std::string indicatorNodeNameInModel, float maxAngle, float maxValue, float minValue = 0.0f);
        Indicator& getIndicator(DesktopIndicatorType type);

        void setButton(DesktopButtonType type, std::string buttonNodeNameInModel, std::vector<glm::vec3>& translateForStates, std::vector<glm::vec3>& rotateForStates, bool isReturning);
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
