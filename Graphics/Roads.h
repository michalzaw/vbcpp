#ifndef ROADS_H_INCLUDED
#define ROADS_H_INCLUDED


#include <vector>
#include <cmath>
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>

#include "Material.h"
#include "Model.h"
#include "NVMeshMender/NVMeshMender.h"


struct RoadLane
{
    Material material;
    float r1;
    float r2;
    float width;
    float height1;
    float height2;

};


struct RoadSegment
{
    glm::vec2 center;
    float r;
    float angle1;
    float angle2;

};


Model* createRoadModel(RoadLane* roadLanes, int lanesCount, RoadSegment& segment);


#endif // ROADS_H_INCLUDED
