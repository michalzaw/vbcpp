cmake_minimum_required(VERSION 3.6)

project(VBCPP1)

set(BIN_NAME vbcpp1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(libs/glew)
add_subdirectory(libs/glfw-3.3)
add_subdirectory(libs/soil)
add_subdirectory(libs/bullet)
add_subdirectory(libs/assimp-4.0.1)
add_subdirectory(libs/freetype)


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
