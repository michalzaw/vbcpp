# scriipt based on https://yonamine.me/modernizing-luas-build-system-with-cmake-and-fetchcontent-3694ca21a634
cmake_minimum_required(VERSION 3.16)
project(lua LANGUAGES C)

file(GLOB LUA_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file(GLOB LUA_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

# Define executables
set(LUA_EXECUTABLE lua)
set(LUAC_EXECUTABLE luac)

# Separate main files
set(LUA_MAIN src/lua.c)
set(LUAC_MAIN src/luac.c)
list(REMOVE_ITEM LUA_SOURCES ${LUA_MAIN} ${LUAC_MAIN})

# Static and shared libraries
add_library(lua_static STATIC ${LUA_SOURCES} ${LUA_HEADERS})
#add_library(lua_shared SHARED ${LUA_SOURCES} ${LUA_HEADERS})
target_compile_features(lua_static PRIVATE c_std_99)
#target_compile_features(lua_shared PRIVATE c_std_99)

# Executables
add_executable(${LUA_EXECUTABLE} ${LUA_MAIN})
add_executable(${LUAC_EXECUTABLE} ${LUAC_MAIN})
target_link_libraries(${LUA_EXECUTABLE} PRIVATE lua_static)
target_link_libraries(${LUAC_EXECUTABLE} PRIVATE lua_static)

if (UNIX)
    target_link_libraries(lua PRIVATE m)
    target_link_libraries(lua_static PRIVATE m)
endif()
