#ifndef ROADOBJECT_H_INCLUDED
#define ROADOBJECT_H_INCLUDED


#include "RenderObject.h"
#include "Roads.h"


class RoadObject : public RenderObject
{
	private:
		std::vector<RoadLane> _roadLanes;
		std::vector<RoadSegment> _segments;

	public:
		RoadObject(std::vector<RoadLane>& roadLanes, std::vector<RoadSegment>& segments);
		virtual ~RoadObject();

		void buildModel();

};


#endif // ROADOBJECT_H_INCLUDED
