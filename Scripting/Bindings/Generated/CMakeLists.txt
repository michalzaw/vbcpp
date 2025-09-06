set (SCRIPTING_GENERATED_SOURCES
	Scripting/Bindings/Generated/GeneratedLuaBinding.cpp
	Scripting/Bindings/Generated/MaterialLuaBindings.cpp
	Scripting/Bindings/Generated/ModelNodeLuaBindings.cpp
	Scripting/Bindings/Generated/PhysicalBodyWheelLuaBindings.cpp
	Scripting/Bindings/Generated/RenderObjectLuaBindings.cpp
	Scripting/Bindings/Generated/ScriptComponentLuaBindings.cpp
	Scripting/Bindings/Generated/SoundComponentLuaBindings.cpp
	Scripting/Bindings/Generated/TransformLuaBindings.cpp
)

set (SCRIPTING_GENERATED_HEADERS 
	Scripting/Bindings/Generated/ComponentsGetters.h
	Scripting/Bindings/Generated/GeneratedLuaBindings.h
	Scripting/Bindings/Generated/MaterialLuaBindings.h
	Scripting/Bindings/Generated/ModelNodeLuaBindings.h
	Scripting/Bindings/Generated/PhysicalBodyWheelLuaBindings.h
	Scripting/Bindings/Generated/RenderObjectLuaBindings.h
	Scripting/Bindings/Generated/ScriptComponentLuaBindings.h
	Scripting/Bindings/Generated/SoundComponentLuaBindings.h
	Scripting/Bindings/Generated/TransformLuaBindings.h
)

set (SCRIPTING_GENERATED_SOURCES ${SCRIPTING_GENERATED_SOURCES} PARENT_SCOPE)
set (SCRIPTING_GENERATED_HEADERS ${SCRIPTING_GENERATED_HEADERS} PARENT_SCOPE)
