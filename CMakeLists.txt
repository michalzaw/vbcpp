cmake_minimum_required(VERSION 3.6)

project(VBCPP)

set(BIN_NAME vbcpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(libs/glew)
add_subdirectory(libs/glfw-3.3)
add_subdirectory(libs/soil)
add_subdirectory(libs/bullet)
add_subdirectory(libs/assimp-4.0.1)
add_subdirectory(libs/freetype)

SET_TARGET_PROPERTIES(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER "Libs/assimp")
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
SET_TARGET_PROPERTIES(soil PROPERTIES FOLDER "Libs/SOIL")

FIND_PACKAGE(OpenAL) 

add_subdirectory(Bus)
source_group("Bus" FILES ${BUS_SOURCES} ${BUS_HEADERS})

add_subdirectory(Game)
source_group("Game" FILES ${GAME_SOURCES} ${GAME_HEADERS})

add_subdirectory(Graphics)
source_group("Graphics" FILES ${GRAPHICS_SOURCES} ${GRAPHICS_HEADERS})

add_subdirectory(GUI)
source_group("GUI" FILES ${GUI_SOURCES} ${GUI_HEADERS})

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
)

source_group("VBCPP_MAIN_GROUP" FILES ${SOURCES})

find_library(glew glew32 PATHS lib)
include_directories (libs/glew/include)

find_library(glfw glfw3 PATHS lib)
include_directories (libs/glfw-3.3/include)

find_library(soil PATHS lib)
include_directories (libs/soil/inc)

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

include_directories (libs/glm/include)

add_executable(${BIN_NAME}
                    ${BUS_SOURCES}
                    ${BUS_HEADERS}
                    ${GAME_SOURCES}
                    ${GAME_HEADERS}
                    ${GRAPHICS_SOURCES} 
                    ${GRAPHICS_HEADERS}
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

set_target_properties (${BIN_NAME} PROPERTIES VS_STARTUP_PROJECT ${BIN_NAME})

target_link_libraries(${BIN_NAME} glfw glew soil BulletCollision BulletDynamics BulletSoftBody LinearMath assimp freetype)
