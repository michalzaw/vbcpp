set (SCENE_SOURCES
	Scene/Component.cpp
	Scene/SceneManager.cpp
	Scene/SceneObject.cpp
	Scene/Skybox.cpp
)

set (SCENE_HEADERS 
	Scene/Component.h
	Scene/SceneManager.h
	Scene/SceneObject.h
	Scene/Skybox.h
	Scene/SoundComponent.h
	Scene/SoundManager.h
)

set (SCENE_SOURCES ${SCENE_SOURCES} PARENT_SCOPE)
set (SCENE_HEADERS ${SCENE_HEADERS} PARENT_SCOPE)