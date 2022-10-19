#ifndef DESKTOP_H_INCLUDED
#define DESKTOP_H_INCLUDED


#include <functional>

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


enum DesktopButtonAction
{
    DBA_DOOR_OPEN,
    DBA_DOOR_CLOSE,
    DBA_DOOR_OPEN_CLOSE,

    DBA_DOOR_GROUP_OPEN,
    DBA_DOOR_GROUP_CLOSE,
    DBA_DOOR_GROUP_OPEN_CLOSE,

    DBA_DOOR_BLOCK,
    DBA_DOOR_UNBLOCK,

    DBA_TOGGLE_HANDBRAKE,

    BUTTON_ACTIONS_COUNT
};

const std::string desktopButtonActionStrings[] = { "doorOpen", "doorClose", "doorOpenClose", "doorGroupOpen", "doorGroupClose", "doorGroupOpenClose", "doorBlock", "doorUnblock", "toggleHandbrake" };
DesktopButtonAction getDesktopButtonActionFromString(std::string name);


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

    std::vector<glm::vec2> rotationCurve;
    glm::vec3 rotationAxis;

    float valueFromVariable;
    float currentValue;

    Indicator()
    {
        modelNode = NULL;
        currentValue = 0.0f;
    }
};


struct DesktopButtonState
{
    glm::vec3 translation;
    float rotation;

    std::function<void(void)> action;
    std::function<void(int)> actionWithParamInt;
    int actionParam;

    DesktopButtonState(glm::vec3 tranlation = glm::vec3(0.0f, 0.0, 0.0f), float rotation = 0.0f)
        : translation(tranlation), rotation(rotation)
    {

    }
};


struct DesktopButton
{
    ModelNode* modelNode;
    unsigned int statesCount;
    glm::vec3 rotationAxis;
    bool isReturning;

    std::vector<DesktopButtonState> states;

    unsigned int currentState;

    float currentRotation;

    DesktopButton()
    {
        modelNode = NULL;
    }

    void setData(ModelNode* modelNode, glm::vec3 rotationAxis, std::vector<DesktopButtonState> states, bool isReturning)
    {
        this->modelNode = modelNode;
        this->statesCount = states.size();
        this->states = states;
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
        ClickableObject* _desktopClickableObject;

        std::vector<Indicator> _indicators;
        std::vector<DesktopButton> _buttons;
        DesktopLight _lights[DESKTOP_LIGHTS_COUNT];

        void catchInput();

    public:
        Desktop(RenderObject* desktopRenderObject, ClickableObject* desktopClickableObject);

        void addIndicator(const std::string& indicatorNodeNameInModel, const std::string& variable, float maxAngle, float maxValue, float minValue = 0.0f, float minAngle = 0.0f, glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f));
        void addIndicator(const std::string& indicatorNodeNameInModel, const std::string& variable, const std::vector<glm::vec2> rotationCurve, glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f));
        
        void addButton(std::string buttonNodeNameInModel, glm::vec3 rotationAxix, std::vector<DesktopButtonState> states, bool isReturning);
        DesktopButton& getButton(int index);

        void setLight(DesktopLightType type, std::string lightNodeNameInModel, glm::vec3 desktopBacklightingColor, glm::vec3 lightColor);
        DesktopLight& getDesktopLight(DesktopLightType type);

        void setButtonState(int index, unsigned int state);
        void clickButton(int index);
        void setLightBacklightingState(DesktopLightType type, bool isEnable);
        void setLightState(DesktopLightType type, bool isEnable);
        void setDesktopBacklightingState(bool isEnable);

        void update(float deltaTime);

};


#endif // DESKTOP_H_INCLUDED
