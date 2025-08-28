#ifndef COMPONENTSGETTERS_H_INCLUDED
#define COMPONENTSGETTERS_H_INCLUDED


{{ #components }}
#include "{{ filePath }}"
{{ /components }}


#define GET_COMPONENT(type, className) "get" #className, [](SceneObject& sceneObject) { return sceneObject.getComponentWithCasting<className>(type); }

#define COMPONENTS_GETTERS  \
{{ #components }}
GET_COMPONENT({{ type }}, {{ className }}){{^last}},{{/last}} \
{{ /components }}


#endif // COMPONENTSGETTERS_H_INCLUDED
