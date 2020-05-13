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

		void buildModel(bool reuseExistingModel = true);

		RRoadProfile* getRoadProfile();
		std::vector<RoadSegment>& getSegments();

		void setRoadProfile(RRoadProfile* roadProfile);
		void setSegments(std::vector<RoadSegment> segments);

};


#endif // ROADOBJECT_H_INCLUDED
