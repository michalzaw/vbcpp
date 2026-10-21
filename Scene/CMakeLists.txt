set (SCENE_SOURCES
	Scene/Component.cpp
	Scene/SceneLoader.cpp
	Scene/SceneManager.cpp
	Scene/SceneObject.cpp
	Scene/SceneObjectCreator.cpp
	Scene/SceneSaver.cpp
	Scene/Events/EventDispatcher.cpp
	Scene/Events/Subscription.cpp
)

set (SCENE_HEADERS 
	Scene/ClickableObject.h
	Scene/Component.h
	Scene/InternalHelperComponent.h
	Scene/SceneLoader.h
	Scene/SceneManager.h
	Scene/SceneObject.h
	Scene/SceneObjectCreator.h
	Scene/SceneSaver.h
	Scene/SoundComponent.h
	Scene/SoundManager.h
	Scene/Events/EventDispatcher.h
	Scene/Events/EventTypes.h
	Scene/Events/ObjectClickedEvent.h
	Scene/Events/ObjectLoadedEvent.h
	Scene/Events/Subscription.h
)

set (SCENE_SOURCES ${SCENE_SOURCES} PARENT_SCOPE)
set (SCENE_HEADERS ${SCENE_HEADERS} PARENT_SCOPE)