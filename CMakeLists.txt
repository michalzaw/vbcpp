cmake_minimum_required(VERSION 3.6)

project(VBCPP)

set(BIN_NAME vbcpp)
set(EDITOR_BIN_NAME editor)

set(CMAKE_CXX_STANDARD 11)

#bin in project main dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#set default value of assimp options
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT")
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "ASSIMP_BUILD_FBX_IMPORTER")

#set default options for freealut
set(BUILD_EXAMPLES OFF CACHE BOOL "BUILD_EXAMPLES" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "BUILD_TESTS" FORCE)

# set variables for freealut
set(OPENAL_INCLUDE_DIR ../openal-soft/include)
set(OPENAL_LIBRARY OpenAL)

add_subdirectory(libs/glew)
add_subdirectory(libs/glfw-3.3)
add_subdirectory(libs/bullet)
add_subdirectory(libs/assimp-4.0.1)
add_subdirectory(libs/freetype)
add_subdirectory(libs/openal-soft)
add_subdirectory(libs/freealut)
#add_subdirectory(libs/stb_image)

#SET_TARGET_PROPERTIES(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "Libs/assimp")
SET_TARGET_PROPERTIES(assimp PROPERTIES FOLDER "Libs/assimp")
SET_TARGET_PROPERTIES(IrrXML PROPERTIES FOLDER "Libs/assimp")
SET_TARGET_PROPERTIES(zlibstatic PROPERTIES FOLDER "Libs/assimp")
SET_TARGET_PROPERTIES(ConvexDecomposition PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(GIMPACTUtils PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(HACD PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(BulletInverseDynamicsUtils PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(BulletFileLoader PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(BulletWorldImporter PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(BulletXmlWorldImporter PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(Bullet3Collision PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(Bullet3Common PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(Bullet3Dynamics PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(Bullet3Geometry PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(Bullet3OpenCL_clew PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(Bullet2FileLoader PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(BulletCollision PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(BulletDynamics PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(BulletInverseDynamics PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(BulletSoftBody PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(LinearMath PROPERTIES FOLDER "Libs/Bullet")
SET_TARGET_PROPERTIES(freetype PROPERTIES FOLDER "Libs/FreeType")
SET_TARGET_PROPERTIES(uninstall PROPERTIES FOLDER "Libs/GLFW3")
SET_TARGET_PROPERTIES(glfw PROPERTIES FOLDER "Libs/GLFW3")
SET_TARGET_PROPERTIES(alrecord PROPERTIES FOLDER "Libs/OpenAL")
SET_TARGET_PROPERTIES(altonegen PROPERTIES FOLDER "Libs/OpenAL")
SET_TARGET_PROPERTIES(common PROPERTIES FOLDER "Libs/OpenAL")
SET_TARGET_PROPERTIES(makehrtf PROPERTIES FOLDER "Libs/OpenAL")
SET_TARGET_PROPERTIES(native-tools PROPERTIES FOLDER "Libs/OpenAL")
SET_TARGET_PROPERTIES(OpenAL PROPERTIES FOLDER "Libs/OpenAL")
SET_TARGET_PROPERTIES(openal-info PROPERTIES FOLDER "Libs/OpenAL")
SET_TARGET_PROPERTIES(alut PROPERTIES FOLDER "Libs/alut")
#SET_TARGET_PROPERTIES(hello_world PROPERTIES FOLDER "Libs/alut")
#SET_TARGET_PROPERTIES(playfile PROPERTIES FOLDER "Libs/alut")
#SET_TARGET_PROPERTIES(test_errorstuff PROPERTIES FOLDER "Libs/alut")
#SET_TARGET_PROPERTIES(test_fileloader PROPERTIES FOLDER "Libs/alut")
#SET_TARGET_PROPERTIES(test_memoryloader PROPERTIES FOLDER "Libs/alut")
#SET_TARGET_PROPERTIES(test_version PROPERTIES FOLDER "Libs/alut")
#SET_TARGET_PROPERTIES(test_waveforms PROPERTIES FOLDER "Libs/alut")

add_subdirectory(Bus)
source_group("Bus" FILES ${BUS_SOURCES} ${BUS_HEADERS})

add_subdirectory(Editor)
source_group("Editor" FILES ${EDITOR_SOURCES} ${EDITOR_HEADERS})

add_subdirectory(Game)
source_group("Game" FILES ${GAME_SOURCES} ${GAME_HEADERS})

add_subdirectory(Graphics)
source_group("Graphics" FILES ${GRAPHICS_SOURCES} ${GRAPHICS_HEADERS})

add_subdirectory(GUI)
source_group("GUI" FILES ${GUI_SOURCES} ${GUI_HEADERS})

add_subdirectory(ImGui)
source_group("ImGui" FILES ${IMGUI_SOURCES} ${IMGUI_HEADERS})

add_subdirectory(ImGuiInterface)
source_group("ImGuiInterface" FILES ${IMGUI_INTERFACE_SOURCES} ${IMGUI_INTERFACE_HEADERS})

add_subdirectory(Physics)
source_group("Physics" FILES ${PHYSICS_SOURCES} ${PHYSICS_HEADERS})

add_subdirectory(Scene)
source_group("Scene" FILES ${SCENE_SOURCES} ${SCENE_HEADERS})

add_subdirectory(Utils)
source_group("Utils" FILES ${UTILS_SOURCES} ${UTILS_HEADERS})

add_subdirectory(Window)
source_group("Window" FILES ${WINDOW_SOURCES} ${WINDOW_HEADERS})

set (SOURCES 
    main.cpp
	Game.h
	Game.cpp
	#EditorMain.h
	#EditorMain.cpp
)

set (EDITOR_MAIN
	EditorMain.cpp
)

source_group("VBCPP_MAIN_GROUP" FILES ${SOURCES})

source_group("EDITOR_MAIN" FILES ${EDITOR_MAIN})

find_library(glew glew32 PATHS lib)
include_directories (libs/glew/include)

find_library(glfw glfw3 PATHS lib)
include_directories (libs/glfw-3.3/include)

find_library(BulletCollision PATHS lib)
find_library(BulletDynamics PATHS lib)
find_library(BulletSoftBody PATHS lib)
find_library(LinearMath PATHS lib)
include_directories (libs/bullet/src)

find_library(assimp PATHS lib)
include_directories (libs/assimp-4.0.1/include)
include_directories (${CMAKE_CURRENT_BINARY_DIR}/libs/assimp-4.0.1/include)

find_library(freetype PATHS lib)
include_directories (libs/freetype/include)

include_directories (libs/gli/external/glm)
include_directories (libs/gli)

find_library(OpenAL PATHS lib)
include_directories (libs/openal-soft/include)

find_library(alut PATHS lib)
include_directories (libs/freealut/include)

include_directories (libs/stb_image)

include_directories (libs/PortableFileDialogs)

add_executable(${BIN_NAME}
                    ${BUS_SOURCES}
                    ${BUS_HEADERS}
                    ${EDITOR_SOURCES}
                    ${EDITOR_HEADERS}
                    ${GAME_SOURCES}
                    ${GAME_HEADERS}
                    ${GRAPHICS_SOURCES} 
                    ${GRAPHICS_HEADERS}
                    ${IMGUI_SOURCES}
                    ${IMGUI_HEADERS}
                    ${IMGUI_INTERFACE_SOURCES}
                    ${IMGUI_INTERFACE_HEADERS}
                    ${GUI_SOURCES}
                    ${GUI_HEADERS}
                    ${PHYSICS_SOURCES}
                    ${PHYSICS_HEADERS}
                    ${SCENE_SOURCES}
                    ${SCENE_HEADERS}
                    ${UTILS_SOURCES}
                    ${UTILS_HEADERS}
                    ${WINDOW_SOURCES}
                    ${WINDOW_HEADERS}
                    ${SOURCES}
)

add_executable(${EDITOR_BIN_NAME}
                    ${BUS_SOURCES}
                    ${BUS_HEADERS}
                    ${EDITOR_SOURCES}
                    ${EDITOR_HEADERS}
                    ${GAME_SOURCES}
                    ${GAME_HEADERS}
                    ${GRAPHICS_SOURCES} 
                    ${GRAPHICS_HEADERS}
                    ${IMGUI_SOURCES}
                    ${IMGUI_HEADERS}
                    ${GUI_SOURCES}
                    ${GUI_HEADERS}
                    ${PHYSICS_SOURCES}
                    ${PHYSICS_HEADERS}
                    ${SCENE_SOURCES}
                    ${SCENE_HEADERS}
                    ${UTILS_SOURCES}
                    ${UTILS_HEADERS}
                    ${WINDOW_SOURCES}
                    ${WINDOW_HEADERS}
                    ${EDITOR_MAIN}
)

set_target_properties (${BIN_NAME} PROPERTIES VS_STARTUP_PROJECT ${BIN_NAME})
set_target_properties (${BIN_NAME} PROPERTIES DEBUG_POSTFIX "_Debug")

#output dir for executable files
set_target_properties(${BIN_NAME} glew alut OpenAL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#VS working dir
set_target_properties(${BIN_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_link_libraries(${BIN_NAME} glfw glew BulletCollision BulletDynamics BulletSoftBody LinearMath assimp freetype OpenAL alut)

#set_target_properties(${EDITOR_BIN_NAME} glew alut OpenAL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#set_target_properties(${EDITOR_BIN_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set_target_properties (${EDITOR_BIN_NAME} PROPERTIES DEBUG_POSTFIX "_Debug")

#output dir for executable files
set_target_properties(${EDITOR_BIN_NAME} glew alut OpenAL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#VS working dir
set_target_properties(${EDITOR_BIN_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

target_link_libraries(${EDITOR_BIN_NAME} glfw glew BulletCollision BulletDynamics BulletSoftBody LinearMath assimp freetype OpenAL alut)



#EDITOR_BIN_NAME