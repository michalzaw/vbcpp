#ifndef ROBJECTSAVER_H_INCLUDED
#define ROBJECTSAVER_H_INCLUDED


#include <string>


class SceneObject;


namespace RObjectSaver
{
	void saveObjectToFile(SceneObject* sceneObject, const std::string& pathToDirectory);

}


#endif // ROBJECTSAVER_H_INCLUDED
