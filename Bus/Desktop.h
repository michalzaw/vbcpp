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


class Desktop
{
    private:
        SceneObject* _desktopSceneObject;
        RenderObject* _desktopRenderObject;

        Indicator _indicators[INDICATORS_COUNT];
        DesktopButton _buttons[BUTTONS_COUNT];

    public:
        Desktop(RenderObject* desktopRenderObject);

        void setIndicator(DesktopIndicatorType type, std::string indicatorNodeNameInModel, float maxAngle, float maxValue, float minValue = 0.0f);
        Indicator& getIndicator(DesktopIndicatorType type);

        void setButton(DesktopButtonType type, std::string buttonNodeNameInModel, std::vector<glm::vec3>& translateForStates, std::vector<glm::vec3>& rotateForStates, bool isReturning);
        DesktopButton& getButton(DesktopButtonType type);

        void setIndicatorValue(DesktopIndicatorType type, float value);
        void setButtonState(DesktopButtonType type, unsigned int state);

        void update(float deltaTime);

};


#endif // DESKTOP_H_INCLUDED
