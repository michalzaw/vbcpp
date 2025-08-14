set (SCRIPTING_SOURCES
	Scripting/RScriptFile.cpp
	Scripting/ScriptComponent.cpp
	Scripting/ScriptingManager.cpp
)

set (SCRIPTING_HEADERS 
	Scripting/RScriptFile.h
	Scripting/ScriptComponent.h
	Scripting/ScriptingManager.h
)

set (SCRIPTING_SOURCES ${SCRIPTING_SOURCES} PARENT_SCOPE)
set (SCRIPTING_HEADERS ${SCRIPTING_HEADERS} PARENT_SCOPE)