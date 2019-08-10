#ifndef ROADOBJECT_H_INCLUDED
#define ROADOBJECT_H_INCLUDED


#include "RenderObject.h"
#include "Roads.h"
#include "RRoadProfile.h"


class RoadObject : public RenderObject
{
	private:
		RRoadProfile* _roadProfile;
		std::vector<RoadSegment> _segments;

	public:
		RoadObject(RRoadProfile*_roadProfile, std::vector<RoadSegment>& segments);
		virtual ~RoadObject();

		void buildModel();

};


#endif // ROADOBJECT_H_INCLUDED
