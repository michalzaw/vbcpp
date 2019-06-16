set (UTILS_SOURCES
	Utils/Collision.cpp
	Utils/Engine.cpp
	Utils/Gearbox.cpp
	Utils/Helpers.cpp
	Utils/RFont.cpp
	Utils/RModel.cpp
	Utils/RStaticModel.cpp
	Utils/RTexture.cpp
	Utils/RTexture2D.cpp
	Utils/RTextureCubeMap.cpp
	Utils/ResourceManager.cpp
	Utils/SoundLoader.cpp
	Utils/tinyxml2.cpp
)

set (UTILS_HEADERS
	Utils/Collision.h
	Utils/Engine.h
	Utils/FilesHelper.h
	Utils/Gearbox.h
	Utils/Math.h
	Utils/Helpers.hpp
	Utils/RFont.h
	Utils/RModel.h
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
)

set (UTILS_SOURCES ${UTILS_SOURCES} PARENT_SCOPE)
set (UTILS_HEADERS ${UTILS_HEADERS} PARENT_SCOPE)