set (EDITOR_SOURCES
    Editor/Editor.cpp
	Editor/EditorGUI.cpp
	Editor/Windows/CameraSettingsWindow.cpp
	Editor/Windows/EditorWindow.cpp
	Editor/Windows/NewDialogWindow.cpp
	Editor/Windows/ObjectPropertiesWindow.cpp
	Editor/Windows/OpenDialogWindow.cpp
	Editor/Windows/SceneGraphWindow.cpp
)

set (EDITOR_HEADERS 
	Editor/Editor.h
	Editor/EditorEvent.h
	Editor/EditorGUI.h
	Editor/Windows/CameraSettingsWindow.h
	Editor/Windows/EditorWindow.h
	Editor/Windows/NewDialogWindow.h
	Editor/Windows/ObjectPropertiesWindow.h
	Editor/Windows/OpenDialogWindow.h
	Editor/Windows/SceneGraphWindow.h
)

set (EDITOR_SOURCES ${EDITOR_SOURCES} PARENT_SCOPE)
set (EDITOR_HEADERS ${EDITOR_HEADERS} PARENT_SCOPE)