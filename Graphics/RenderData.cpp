#include "RenderData.h"


RenderListElement::RenderListElement(RModel* model, Mesh* mesh, TransformMatrices matrices, float distance, SceneObject* object)
    : _model(model), _mesh(mesh), _matrices(matrices), _distanceFromCamera(distance), _object(object)
{

}
RenderListElement::~RenderListElement()
{

}
