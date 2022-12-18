#ifndef ROADINTERSECTIONCOMPONENT_H_INCLUDED
#define ROADINTERSECTIONCOMPONENT_H_INCLUDED


#include <vector>

#include "../Scene/Component.h"


class BezierCurve;
class RoadObject;
class RStaticModel;
class RRoadProfile;
class Material;


struct RoadConnectedToIntersection final
{
	RoadObject* road;
	BezierCurve* bezierCurve;
	int connectionPointInRoadIndex;

	RoadConnectedToIntersection(RoadObject* road, BezierCurve* bezierCurve, int connectionPointInRoadIndex)
		: road(road), bezierCurve(bezierCurve), connectionPointInRoadIndex(connectionPointInRoadIndex)
	{}
};


class RoadIntersectionComponent final : public Component
{
	private:
		static const unsigned int DEFAULT_BUFFER_SIZE = 1024 * 1024; // ~ 4600 vertices

		std::vector<RoadConnectedToIntersection> _roads;

		std::vector<float> _length;
		std::vector<float> _width;
		std::vector<float> _arc;
		int _quality;

		RRoadProfile* _originalEdgeRoadProfile;
		RRoadProfile* _edgeRoadProfile;
		unsigned int _edgeRoadProfileNumberOfLanesToRemove;		// liczba lane ktore beda odrzucane przy tworzeniu krawedzi skrzyzowania, patrzac od osi jezdni. Default = 1

		bool _interactiveMode;

		bool _needRebuildIntersectionModel;
		bool _needRebuildConnectedRoad;
		bool _needSortRoads;
		bool _needRecreateModel;
		float _modificationTimer;

		glm::mat4 _inverseModelMatrix;
		mutable bool _inverseModelMatrixIsCalculated;

		RStaticModel* _generatedModel;

		bool _isCreated;

		void onAttachedToScenObject() override;

		void createDebugPolygonComponent(const std::vector<std::vector<glm::vec3>>& pointsOnRoadAxis, const std::vector<std::vector<glm::vec3>>& bezierCurves);
		float getRealWidth(int index, bool isRight);
		void setLengthInternal(int index, float length);
		void sortConnectedRoads();
		const glm::mat4& getInverseModelMatrix();
		glm::vec3 transformPointToLocalSpace(const glm::vec3& point);
		glm::vec3 transformVectorToLocalSpace(const glm::vec3& point);
		void updateEdgeRoadProfile();

	public:
		RoadIntersectionComponent(RRoadProfile* edgeRadProfile, bool interactiveMode);
		virtual ~RoadIntersectionComponent();

		void connectRoad(RoadObject* roadObject, int connectionPointInRoadIndex);
		void disconnectRoad(RoadObject* roadObject, int connectionPointInRoadIndex);

		const std::vector<RoadConnectedToIntersection>& getConnectedRoads();

		void setLength(float length);
		void setLength(int index, float length, bool modifyAll = false);
		void setWidth(float width);
		void setWidth(int index, float width, bool modifyAll = false);
		void setArc(float arc);
		void setArc(int index, float arc, bool modifyAll = false);
		void setQuality(int quality);
		float getLength(int index = 0);
		float getWidth(int index = 0);
		float getArc(int index = 0);
		int getQuality();

		void setEdgeRoadProfile(RRoadProfile* roadProfile);
		void setEdgeRoadProfileNumberOfLanesToRemove(unsigned int edgeRoadProfileNumberOfLanesToRemove);
		RRoadProfile* getEdgeRoadProfile();
		unsigned int getEdgeRoadProfileNumberOfLanesToRemove();

		void setInteractiveMode(bool interactiveMode);
		bool isInteractiveMode();

		void createPolygon();

		RStaticModel* getModel();

		void needRebuild();
		void needRebuildConnectedRoad();

		void changedTransform() override;
		void update(float deltaTime) override;

};


#endif // ROADINTERSECTIONCOMPONENT_H_INCLUDED
