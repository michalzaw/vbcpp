#ifndef TREECOMPONENT_H_INCLUDED
#define TREECOMPONENT_H_INCLUDED


#include <cstdlib>

#include "../Scene/Component.h"


class TreeComponent : public Component
{
    private:
        glm::vec3 _windDirectionFactor;
        float _windVelocityFactor;

    public:
        TreeComponent()
            : Component(CT_TREE_COMPONENT)
        {
            setRandomWindFactor();
        }

        ~TreeComponent();

        void setRandomWindFactor()
        {
            _windDirectionFactor = glm::vec3((rand() % 21 - 10) / 10.0f, 0.0f, (rand() % 21 - 10) / 10.0f);
            _windVelocityFactor = (rand() % 21 - 10) / 10.0f;
        }

        glm::vec3 getWindVector()
        {
            // todo: implement, temporary unused
            return glm::vec3(0.0f, 0.0f, 0.0f);
            //return (GraphicsManager::getInstance().getWindDirection() + _windDirectionFactor)
            //        * sinf(GraphicsManager::getInstance().getWindValue() + _windVelocityFactor);

        }

};


#endif // TREECOMPONENT_H_INCLUDED
