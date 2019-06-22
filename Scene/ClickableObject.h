#ifndef CLICKABLEOBJECT_H_INCLUDED
#define CLICKABLEOBJECT_H_INCLUDED


#include "Component.h"

#include "../Graphics/ModelNode.h"


class ClickableObject : public Component
{
    private:
        bool _isClicked;

        std::vector<ModelNode*> _clickedNodes;

    public:
        ClickableObject()
            : Component(CT_CLICKABLE_OBJECT)
        {
            clear();
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
