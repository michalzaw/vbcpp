set (EDITOR_SOURCES
    Editor/Editor.cpp
	Editor/EditorGUI.cpp
	Editor/FileDialogs.cpp
	Editor/Windows/CameraSettingsWindow.cpp
	Editor/Windows/EditorWindow.cpp
	Editor/Windows/NewDialogWindow.cpp
	Editor/Windows/ObjectDefinitionsWindow.cpp
	Editor/Windows/ObjectPropertiesWindow.cpp
	Editor/Windows/OpenDialogWindow.cpp
	Editor/Windows/SceneGraphWindow.cpp
	Editor/Windows/RoadTools.cpp
	Editor/Windows/MapInfoWindow.cpp
	Editor/Windows/MaterialEditorWindow.cpp
	Editor/Windows/GenerateObjectsAlongCurveWindow.cpp
	Editor/Windows/LoggerWindow.cpp
	Editor/Windows/ComputeShaderTestWindow.cpp
	Editor/Tools/AxisTool.cpp
	Editor/Tools/RoadManipulator.cpp
	Editor/Utils/AIPathGenerator.cpp
	Editor/Utils/ObjectsGenerator.cpp
	Editor/Utils/RoadPolygonsGenerator.cpp
)

set (EDITOR_HEADERS 
	Editor/Editor.h
	Editor/EditorContext.h
	Editor/EditorEvent.h
	Editor/EditorGUI.h
	Editor/FileDialogs.h
	Editor/Windows/CameraSettingsWindow.h
	Editor/Windows/EditorWindow.h
	Editor/Windows/NewDialogWindow.h
	Editor/Windows/ObjectDefinitionsWindow.h
	Editor/Windows/ObjectPropertiesWindow.h
	Editor/Windows/OpenDialogWindow.h
	Editor/Windows/SceneGraphWindow.h
	Editor/Windows/RoadTools.h
	Editor/Windows/MapInfoWindow.h
	Editor/Windows/MaterialEditorWindow.h
	Editor/Windows/GenerateObjectsAlongCurveWindow.h
	Editor/Windows/LoggerWindow.h
	Editor/Windows/ComputeShaderTestWindow.h
	Editor/Tools/AxisTool.h
	Editor/Tools/RoadManipulator.h
	Editor/Utils/AIPathGenerator.h
	Editor/Utils/ObjectsGenerator.h
	Editor/Utils/RoadPolygonsGenerator.h
)

set (EDITOR_SOURCES ${EDITOR_SOURCES} PARENT_SCOPE)
set (EDITOR_HEADERS ${EDITOR_HEADERS} PARENT_SCOPE)