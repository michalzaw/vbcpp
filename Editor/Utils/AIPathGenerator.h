#ifndef AIPATHGENERATOR_H_INCLUDED
#define AIPATHGENERATOR_H_INCLUDED


class RoadIntersectionComponent;
class RoadObject;
class SceneManager;


namespace AIPathGenerator
{
	void generateAIPaths(RoadObject* roadComponent, SceneManager* sceneManager);
	void generateAIPaths(RoadIntersectionComponent* roadIntersectionComponent, SceneManager* sceneManager,
		int pathQuality = 20, float innerPathCurveFactor = 2.0f, float outerPathCurveFactor = 1.0f,
		bool connectWithExistingPaths = true, float maxDistanceToCreateConnection = 5.0f,
		float speedOnStraightPaths = 40.0f, float speedOnCurvedPaths = 20.0f);
}


#endif // AIPATHGENERATOR_H_INCLUDED
