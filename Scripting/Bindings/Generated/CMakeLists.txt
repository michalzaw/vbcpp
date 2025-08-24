set (SCRIPTING_GENERATED_SOURCES
	Scripting/Bindings/Generated/GeneratedLuaBinding.cpp
	Scripting/Bindings/Generated/PhysicalBodyWheelLuaBindings.cpp
	Scripting/Bindings/Generated/RenderObjectLuaBindings.cpp
)

set (SCRIPTING_GENERATED_HEADERS 
	Scripting/Bindings/Generated/GeneratedLuaBindings.h
	Scripting/Bindings/Generated/PhysicalBodyWheelLuaBindings.h
	Scripting/Bindings/Generated/RenderObjectLuaBindings.h
)

set (SCRIPTING_GENERATED_SOURCES ${SCRIPTING_GENERATED_SOURCES} PARENT_SCOPE)
set (SCRIPTING_GENERATED_HEADERS ${SCRIPTING_GENERATED_HEADERS} PARENT_SCOPE)
