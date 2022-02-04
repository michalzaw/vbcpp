#ifndef ROADINTERSECTIONCOMPONENT_H_INCLUDED
#define ROADINTERSECTIONCOMPONENT_H_INCLUDED


#include <vector>

#include "../Scene/Component.h"


class RoadObject;
class RStaticModel;


struct RoadConnectedToIntersection final
{
	RoadObject* road;
	int connectionPointInRoadIndex;

	RoadConnectedToIntersection(RoadObject* road, int connectionPointInRoadIndex)
		: road(road), connectionPointInRoadIndex(connectionPointInRoadIndex)
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

		bool _needRebuildIntersectionModel;
		bool _needRebuildConnectedRoad;
		bool _needSortRoads;
		float _modificationTimer;

		glm::mat4 _inverseModelMatrix;
		mutable bool _inverseModelMatrixIsCalculated;

		RStaticModel* _generatedModel;

		bool _isCreated;

		void createDebugPolygonComponent(const std::vector<std::vector<glm::vec3>>& pointsOnRoadAxis, const std::vector<std::vector<glm::vec3>>& bezierCurves);
		float getRealWidth(int index, bool isRight);
		void setLengthInternal(int index, float length);
		void sortConnectedRoads();
		const glm::mat4& getInverseModelMatrix();
		glm::vec3 transformPointToLocalSpace(const glm::vec3& point);
		glm::vec3 transformVectorToLocalSpace(const glm::vec3& point);

	public:
		RoadIntersectionComponent();
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

		void createPolygon();

		RStaticModel* getModel();

		void needRebuild();
		void needRebuildConnectedRoad();

		void changedTransform() override;
		void update(float deltaTime) override;

};


#endif // ROADINTERSECTIONCOMPONENT_H_INCLUDED
