#ifndef INTERNALHELPERCOMPONENT_H_INCLUDED
#define INTERNALHELPERCOMPONENT_H_INCLUDED


#include "Component.h"


class InternalHelperComponent : public Component
{
    VBCPP_COMPONENT(InternalHelperComponent, CT_INTERNAL_HELPER)

    private:
        SceneObject* _referenceObject;

    public:
        InternalHelperComponent(SceneObject* referenceObject)
            : Component(CT_INTERNAL_HELPER),
            _referenceObject(referenceObject)
        {

        }

        inline SceneObject* getReferenceObject() { return _referenceObject; }

};


#endif // INTERNALHELPERCOMPONENT_H_INCLUDED
