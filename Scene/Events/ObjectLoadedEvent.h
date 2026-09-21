#ifndef OBJECTLOADEDEVENT_H_INCLUDED
#define OBJECTLOADEDEVENT_H_INCLUDED


#include "EventTypes.h"

#include "../../Scripting/Utils/LuaMacros.h"


class SceneObject;


// Event wystepujacy po zakonczeniu wczytywania calego obiektu z pliku object.xml lub config.xml (w przypadku autobusu)
// Obieckt jest wtedy utworzony i posiada wszystkie swoje componenty i dzieci zgodnie z definicja z pliku konfiguracyjnego
VBCPP_EVENT
struct ObjectLoadedEvent
{
	LUAV SceneObject* loadedObject;
};


#endif // OBJECTLOADEDEVENT_H_INCLUDED
