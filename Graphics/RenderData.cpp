#include "RenderData.h"


RenderListElement::RenderListElement(RenderElementType type, RStaticModel* model, StaticModelMesh* mesh, TransformMatrices matrices, float distance, SceneObject* object, RenderObject* renderObject)
    : _type(type), _model(model), _mesh(mesh), _matrices(matrices), _distanceFromCamera(distance), _object(object), _renderObject(renderObject)
{

}
RenderListElement::~RenderListElement()
{

}
