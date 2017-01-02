#include "RenderData.h"


RenderListElement::RenderListElement(RModel* model, Mesh* mesh, TransformMatrices matrices, float distance)
    : _model(model), _mesh(mesh), _matrices(matrices), _distanceFromCamera(distance)
{

}
RenderListElement::~RenderListElement()
{

}
