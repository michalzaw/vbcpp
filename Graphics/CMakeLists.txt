set (GRAPHICS_SOURCES
		Graphics/AABB.h
		Graphics/AnimatedModelLoader.cpp
		Graphics/AnimationLoader.cpp
		Graphics/BezierCurve.cpp
		Graphics/Camera.cpp
		Graphics/Camera.h
		Graphics/CameraFPS.cpp
		Graphics/CameraFPS.hpp
		Graphics/CameraStatic.cpp
		Graphics/CameraStatic.hpp
		Graphics/CrossroadComponent.cpp
		Graphics/CrossroadComponent.h
		Graphics/DisplayComponent.cpp
		Graphics/DisplayComponent.h
		Graphics/EnvironmentCaptureComponent.cpp
		Graphics/EnvironmentCaptureComponent.h
		Graphics/Framebuffer.cpp
		Graphics/Framebuffer.h
		Graphics/Frustum.cpp
		Graphics/Frustum.h
		Graphics/GraphicsManager.cpp
		Graphics/GraphicsManager.h
		Graphics/Grass.cpp
		Graphics/Grass.h
		Graphics/IBO.cpp
		Graphics/IBO.h
		Graphics/Light.cpp
		Graphics/Light.h
		Graphics/LoadMaterial.cpp
		Graphics/LoadMaterial.h
		Graphics/LoadShader.cpp
		Graphics/LoadShader.h
		Graphics/LoadTerrainModel.cpp
		Graphics/LoadTerrainModel.h
		Graphics/LoadTexture.cpp
		Graphics/LoadTexture.h
		Graphics/Material.h
		Graphics/MaterialSaver.cpp
		Graphics/MaterialSaver.h
		Graphics/MirrorComponent.h
		Graphics/Model.cpp
		Graphics/Model.h
		Graphics/ModelGenerator.cpp
		Graphics/ModelGenerator.h
		Graphics/ModelNode.cpp
		Graphics/ModelNode.h
		Graphics/NVMeshMender/NVMeshMender.cpp
		Graphics/NVMeshMender/NVMeshMender.h
		Graphics/OGLDriver.cpp
		Graphics/OGLDriver.h
		Graphics/PhysicsDebugRenderer.cpp
		Graphics/PhysicsDebugRenderer.h
		Graphics/PostProcessingBloom.cpp
		Graphics/PostProcessingBloom.h
		Graphics/PostProcessingEffect.cpp
		Graphics/PostProcessingEffect.h
		Graphics/PostProcessingToneMapping.h
		Graphics/Prefab.h
		Graphics/Prefab.cpp
		Graphics/QuadTree.cpp
		Graphics/QuadTree.h
		Graphics/QuadTreeNode.cpp
		Graphics/QuadTreeNode.h
		Graphics/RAnimation.cpp
		Graphics/RDisplayFont.cpp
		Graphics/RDisplayFont.h
		Graphics/RRoadProfile.cpp
		Graphics/RRoadProfile.h
		Graphics/RShader.h
		Graphics/Rect.h
		Graphics/RenderData.cpp
		Graphics/RenderData.h
		Graphics/RenderObject.cpp
		Graphics/RenderObject.h
		Graphics/RMaterialsCollection.cpp
		Graphics/RMaterialsCollection.h
		Graphics/RoadGenerator.cpp
		Graphics/RoadGenerator.h
		Graphics/RoadIntersectionComponent.cpp
		Graphics/RoadIntersectionComponent.h
		Graphics/RoadObject.cpp
		Graphics/RoadObject.h
		Graphics/RoadProfileLoader.cpp
		Graphics/RoadProfileLoader.h
		Graphics/Renderer.cpp
		Graphics/Renderer.h
		Graphics/Roads.cpp
		Graphics/Roads.h
		Graphics/RotationMode.h
		Graphics/ShadowMap.cpp
		Graphics/ShadowMap.h
		Graphics/ShapePolygonComponent.cpp
		Graphics/ShapePolygonComponent.h
		Graphics/SkeletalAnimationComponent.cpp
		Graphics/SkeletalAnimationComponent2.cpp
		Graphics/SkeletalAnimationHelperComponent.cpp
		Graphics/Sky.h
		Graphics/StaticModelLoader.cpp
		Graphics/StaticModelLoader.h
		Graphics/Terrain.cpp
		Graphics/Terrain.h
		Graphics/Transform.cpp
		Graphics/Transform.h
		Graphics/TreeComponent.h
		Graphics/UBO.h
		Graphics/VAO.h
		Graphics/VBO.cpp
		Graphics/Vertex.cpp
)

set (GRAPHICS_HEADERS
		Graphics/StorageBuffer/PickingShaderStorageBufferData.h
		Graphics/AnimatedModelLoader.h
		Graphics/AnimationLoader.h
		Graphics/BezierCurve.h
		Graphics/RAnimation.h
		Graphics/AnimationNodeData.h
		Graphics/ShaderStorageBuffer.h
		Graphics/SkeletalAnimationComponent.h
		Graphics/SkeletalAnimationComponent2.h
		Graphics/SkeletalAnimationHelperComponent.h
		Graphics/VBO.h
		Graphics/Vector3.h
		Graphics/Vertex.h
)

set (GRAPHICS_SOURCES ${GRAPHICS_SOURCES} PARENT_SCOPE)
set (GRAPHICS_HEADERS ${GRAPHICS_HEADERS} PARENT_SCOPE)