#include "RenderData.h"


RenderListElement::RenderListElement(Model* model, Mesh* mesh, Transform* transform, float distance)
    : _model(model), _mesh(mesh), _transform(transform), _distanceFromCamera(distance)
{

}
RenderListElement::~RenderListElement()
{

}
