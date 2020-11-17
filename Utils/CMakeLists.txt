set (UTILS_SOURCES
	Utils/BezierCurvesUtils.cpp
	Utils/Collision.cpp
	Utils/Engine.cpp
	Utils/FilesHelper.cpp
	Utils/Gearbox.cpp
	Utils/Helpers.cpp
	Utils/RaycastingUtils.cpp
	Utils/RFont.cpp
	Utils/RModel.cpp
	Utils/RObject.cpp
	Utils/RObjectLoader.cpp
	Utils/RStaticModel.cpp
	Utils/RTexture.cpp
	Utils/RTexture2D.cpp
	Utils/RTextureCubeMap.cpp
	Utils/ResourceManager.cpp
	Utils/SoundLoader.cpp
	Utils/stb_image.cpp
	Utils/Strings.cpp
	Utils/tinyxml2.cpp
	Utils/XmlUtils.cpp
)

set (UTILS_HEADERS
	Utils/BezierCurvesUtils.h
	Utils/Collision.h
	Utils/Engine.h
	Utils/FilesHelper.h
	Utils/Gearbox.h
	Utils/Helpers.hpp
	Utils/Logger.h
	Utils/Math.h
	Utils/RaycastingUtils.h
	Utils/RFont.h
	Utils/RModel.h
	Utils/RObject.h
	Utils/RObjectLoader.h
	Utils/RSound.h
	Utils/RStaticModel.h
	Utils/RTexture.h
	Utils/RTexture2D.h
	Utils/RTextureCubeMap.h
	Utils/RefCounter.h
	Utils/Resource.h
	Utils/ResourceManager.h
	Utils/SoundLoader.h
	Utils/Strings.h
	Utils/Timer.h
	Utils/tinyxml2.h
	Utils/XmlUtils.h
)

set (UTILS_SOURCES ${UTILS_SOURCES} PARENT_SCOPE)
set (UTILS_HEADERS ${UTILS_HEADERS} PARENT_SCOPE)