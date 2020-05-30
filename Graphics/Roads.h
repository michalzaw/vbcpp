#ifndef ROADS_H_INCLUDED
#define ROADS_H_INCLUDED


#include <vector>
#include <cmath>
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>

#include "Material.h"
#include "NVMeshMender/NVMeshMender.h"

#include "../Utils/Math.h"
#include "../Utils/RStaticModel.h"


/*struct RoadLane
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

};*/


struct RoadLane
{
    Material material;
    float r1;           // x1
    float r2;           // x2
    float height1;      // y1
    float height2;      // y2

};


enum RoadInterpolation
{
    RI_LIN,
    RI_COS

};

enum RoadSegmetType
{
    RST_ARC,
    RST_LINE

};


struct RoadSegment
{
    RoadSegmetType type;
    float r;
    glm::vec3 begin;
    glm::vec3 end;
    int pointsCount;
    RoadInterpolation interpolation;

    RoadSegment()
        : type(RST_LINE), r(0), begin(0.0f, 0.0f, 0.0f), end(0.0f, 0.0f, 0.0f), pointsCount(10), interpolation(RI_LIN)
    {}

};


RStaticModel* createRoadModel(std::vector<RoadLane>& roadLanes, std::vector<RoadSegment>& segments, RStaticModel* oldModel = NULL);


#endif // ROADS_H_INCLUDED
