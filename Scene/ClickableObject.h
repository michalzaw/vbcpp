#ifndef CLICKABLEOBJECT_H_INCLUDED
#define CLICKABLEOBJECT_H_INCLUDED


#include "Component.h"

#include "../Graphics/ModelNode.h"

#include "Events/ObjectClickedEvent.h"


class ClickableObject : public Component
{
    VBCPP_COMPONENT(ClickableObject, CT_CLICKABLE_OBJECT)

    private:
        bool _isClicked;

        std::vector<ModelNode*> _clickedNodes;
        
        Subscription _objectClickedEventSubscription;

	protected:
        void onAttachedToScenObject() override
        {
            _objectClickedEventSubscription = getSceneObject()->getEvents().subscribe<ObjectClickedEvent>(std::bind(&ClickableObject::onObjectClicked, this, std::placeholders::_1));
        }

    public:
        ClickableObject()
            : Component(CT_CLICKABLE_OBJECT)
        {
            clear();
        }

        void onObjectClicked(ObjectClickedEvent event)
        {
            click(event.modelNode);
        }

        void click(ModelNode* clickedNode)
        {
            _isClicked = true;
            _clickedNodes.push_back(clickedNode);
        }

        void clear()
        {
            _isClicked = false;
            _clickedNodes.clear();

        }

        bool isClicked()
        {
            return _isClicked;
        }

        std::vector<ModelNode*>& getClickedNodes()
        {
            return _clickedNodes;
        }

};


#endif // CLICKABLEOBJECT_H_INCLUDED
