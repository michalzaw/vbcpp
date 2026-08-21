set (SCRIPTING_SOURCES
	Scripting/Bindings/BusRoutesLuaBindings.cpp
	Scripting/Bindings/ComponentLuaBindings.cpp
	Scripting/Bindings/DisplayTextTypeLuaBindings.cpp
	Scripting/Bindings/GlmLuaBindings.cpp
	Scripting/Bindings/InputSystemLuaBindings.cpp
	Scripting/Bindings/LoggerLuaBindings.cpp
	Scripting/Bindings/LuaBindings.cpp
	Scripting/Bindings/PhysicalBodyTypeLuaBindings.cpp
	Scripting/Bindings/SceneManagerLuaBindings.cpp
	Scripting/Bindings/SceneObjectLuaBindings.cpp
	Scripting/Bindings/ScheduleLuaBindings.cpp
	Scripting/Utils/LuaUtils.cpp
	Scripting/RScriptFile.cpp
	Scripting/ScriptComponent.cpp
	Scripting/ScriptingManager.cpp
)

set (SCRIPTING_HEADERS 
	Scripting/Bindings/BusRoutesLuaBindings.h
	Scripting/Bindings/ComponentLuaBindings.h
	Scripting/Bindings/DisplayTextTypeLuaBindings.h
	Scripting/Bindings/GlmLuaBindings.h
	Scripting/Bindings/InputSystemLuaBindings.h
	Scripting/Bindings/LoggerLuaBindings.h
	Scripting/Bindings/LuaBindings.h
	Scripting/Bindings/PhysicalBodyTypeLuaBindings.h
	Scripting/Bindings/SceneManagerLuaBindings.h
	Scripting/Bindings/SceneObjectLuaBindings.h
	Scripting/Bindings/ScheduleLuaBindings.h
	Scripting/Utils/LuaUtils.h
	Scripting/Utils/LuaMacros.h
	Scripting/RScriptFile.h
	Scripting/ScriptComponent.h
	Scripting/ScriptingManager.h
)

set (SCRIPTING_SOURCES ${SCRIPTING_SOURCES} PARENT_SCOPE)
set (SCRIPTING_HEADERS ${SCRIPTING_HEADERS} PARENT_SCOPE)