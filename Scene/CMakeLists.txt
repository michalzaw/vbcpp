set (SCENE_SOURCES
	Scene/Component.cpp
	Scene/SceneLoader.cpp
	Scene/SceneManager.cpp
	Scene/SceneObject.cpp
	Scene/SceneSaver.cpp
	Scene/Skybox.cpp
)

set (SCENE_HEADERS 
	Scene/ClickableObject.h
	Scene/Component.h
	Scene/SceneLoader.h
	Scene/SceneManager.h
	Scene/SceneObject.h
	Scene/SceneSaver.cpp
	Scene/Skybox.h
	Scene/SoundComponent.h
	Scene/SoundManager.h
)

set (SCENE_SOURCES ${SCENE_SOURCES} PARENT_SCOPE)
set (SCENE_HEADERS ${SCENE_HEADERS} PARENT_SCOPE)