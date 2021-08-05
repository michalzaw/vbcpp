set (GUI_SOURCES
	GUI/Button.cpp
	GUI/FontLoader.cpp
	GUI/GUIManager.cpp
	GUI/GUIObject.cpp
	GUI/GUIRenderListElement.cpp
	GUI/Image.cpp
	GUI/Label.cpp
	GUI/ProgressBar.cpp
)

set (GUI_HEADERS 
	GUI/Button.h
	GUI/FontLoader.h
	GUI/GUIManager.h
	GUI/GUIObject.h
	GUI/GUIRenderListElement.h
	GUI/Image.h
	GUI/Label.h
	GUI/ProgressBar.h
)

set (GUI_SOURCES ${GUI_SOURCES} PARENT_SCOPE)
set (GUI_HEADERS ${GUI_HEADERS} PARENT_SCOPE)