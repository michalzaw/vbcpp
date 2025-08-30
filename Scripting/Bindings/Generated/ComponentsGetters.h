// Generated file
#ifndef COMPONENTSGETTERS_H_INCLUDED
#define COMPONENTSGETTERS_H_INCLUDED


#include "../../../Game/AI/AIAgent.h"
#include "../../../Game/AI/AIAgentPhysicalVechicle.h"
#include "../../../Game/AI/AIAgentVehicle.h"
#include "../../../Game/AI/PathComponent.h"
#include "../../../Game/AI/StopComponent.h"
#include "../../../Game/AI/TrafficLightsComponent.h"
#include "../../../Game/BusStartPoint.h"
#include "../../../Game/BusStopComponent.h"
#include "../../../Game/CameraControlComponent.h"
#include "../../../Graphics/BezierCurve.h"
#include "../../../Graphics/CameraStatic.hpp"
#include "../../../Graphics/CrossroadComponent.h"
#include "../../../Graphics/DisplayComponent.h"
#include "../../../Graphics/EnvironmentCaptureComponent.h"
#include "../../../Graphics/Grass.h"
#include "../../../Graphics/Light.h"
#include "../../../Graphics/MirrorComponent.h"
#include "../../../Graphics/Prefab.h"
#include "../../../Graphics/RenderObject.h"
#include "../../../Graphics/RoadIntersectionComponent.h"
#include "../../../Graphics/RoadObject.h"
#include "../../../Graphics/ShapePolygonComponent.h"
#include "../../../Graphics/SkeletalAnimationComponent.h"
#include "../../../Graphics/SkeletalAnimationComponent2.h"
#include "../../../Graphics/SkeletalAnimationHelperComponent.h"
#include "../../../Graphics/Sky.h"
#include "../../../Graphics/Terrain.h"
#include "../../../Graphics/TreeComponent.h"
#include "../../../Physics/PhysicalBody.hpp"
#include "../../../Physics/PhysicalBodyBox.hpp"
#include "../../../Physics/PhysicalBodyBvtTriangleMesh.hpp"
#include "../../../Physics/PhysicalBodyConvexHull.hpp"
#include "../../../Physics/PhysicalBodyCylinder.hpp"
#include "../../../Physics/PhysicalBodyGhost.h"
#include "../../../Physics/PhysicalBodyRaycastVehicle.h"
#include "../../../Physics/PhysicalBodySphere.hpp"
#include "../../../Physics/PhysicalBodyStaticPlane.hpp"
#include "../../../Physics/PhysicalBodyWheel.h"
#include "../../../Scene/ClickableObject.h"
#include "../../../Scene/InternalHelperComponent.h"
#include "../../../Scene/SoundComponent.h"
#include "../../../Scripting/ScriptComponent.h"


#define GET_COMPONENT(type, className) "get" #className, [](SceneObject& sceneObject) { return sceneObject.getComponentWithCasting<className>(type); }

#define COMPONENTS_GETTERS  \
GET_COMPONENT(CT_AI_AGENT, AIAgent), \
GET_COMPONENT(CT_AI_AGENT, AIAgentPhysicalVechicle), \
GET_COMPONENT(CT_AI_AGENT_VEHICLE, AIAgentVehicle), \
GET_COMPONENT(CT_PATH, PathComponent), \
GET_COMPONENT(CT_STOP_COMPONENT, StopComponent), \
GET_COMPONENT(CT_TRAFFIC_LIGHTS, TrafficLightsComponent), \
GET_COMPONENT(CT_BUS_START_POINT, BusStartPoint), \
GET_COMPONENT(CT_BUS_STOP, BusStopComponent), \
GET_COMPONENT(CT_CAMERA_CONTROL, CameraControlComponent), \
GET_COMPONENT(CT_BEZIER_CURVE, BezierCurve), \
GET_COMPONENT(CT_CAMERA, CameraStatic), \
GET_COMPONENT(CT_CROSSROAD, CrossroadComponent), \
GET_COMPONENT(CT_DISPLAY, DisplayComponent), \
GET_COMPONENT(CT_ENVIRONMENT_CAPTURE_COMPONENT, EnvironmentCaptureComponent), \
GET_COMPONENT(CT_GRASS, Grass), \
GET_COMPONENT(CT_LIGHT, Light), \
GET_COMPONENT(CT_MIRROR, MirrorComponent), \
GET_COMPONENT(CT_PREFAB, Prefab), \
GET_COMPONENT(CT_RENDER_OBJECT, RenderObject), \
GET_COMPONENT(CT_ROAD_INTERSECTION, RoadIntersectionComponent), \
GET_COMPONENT(CT_ROAD_OBJECT, RoadObject), \
GET_COMPONENT(CT_SHAPE_POLYGON, ShapePolygonComponent), \
GET_COMPONENT(CT_SKELETAL_ANIMATION, SkeletalAnimationComponent), \
GET_COMPONENT(CT_SKELETAL_ANIMATION_2, SkeletalAnimationComponent2), \
GET_COMPONENT(CT_SKELETAL_ANIMATION_HELPER, SkeletalAnimationHelperComponent), \
GET_COMPONENT(CT_SKY, Sky), \
GET_COMPONENT(CT_TERRAIN, Terrain), \
GET_COMPONENT(CT_TREE_COMPONENT, TreeComponent), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBody), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodyBox), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodyBvtTriangleMesh), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodyConvexHull), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodyCylinder), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodyGhost), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodyRaycastVehicle), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodySphere), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodyStaticPlane), \
GET_COMPONENT(CT_PHYSICAL_BODY, PhysicalBodyWheel), \
GET_COMPONENT(CT_CLICKABLE_OBJECT, ClickableObject), \
GET_COMPONENT(CT_INTERNAL_HELPER, InternalHelperComponent), \
GET_COMPONENT(CT_SOUND, SoundComponent), \
GET_COMPONENT(CT_SCRIPT, ScriptComponent) \


#endif // COMPONENTSGETTERS_H_INCLUDED
