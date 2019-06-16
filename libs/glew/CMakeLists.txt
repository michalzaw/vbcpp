cmake_minimum_required(VERSION 3.6)

project(Glew)

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (GLEW_LIB_NAME glew32)
set (DLL_PREFIX lib)
set (GLEW_SOURCES src/glew.c)
set (GLEW_HEADERS include/GL/glew.h include/GL/glxew.h include/GL/wglew.h)

find_package (OpenGL REQUIRED)

add_library(glew SHARED ${GLEW_HEADERS} ${GLEW_SOURCES})
set_target_properties (glew PROPERTIES COMPILE_DEFINITIONS "GLEW_BUILD" OUTPUT_NAME "${GLEW_LIB_NAME}" PREFIX "${DLL_PREFIX}" FOLDER "Libs/GLEW")

include_directories (include)

if (MSVC)
  # add options from visual studio project
  target_compile_definitions (glew PRIVATE "GLEW_BUILD;VC_EXTRALEAN")
  target_link_libraries (glew LINK_PRIVATE -BASE:0x62AA0000)
  # kill security checks which are dependent on stdlib
  target_compile_options (glew PRIVATE -GS-)
  # remove stdlib dependency
  target_link_libraries (glew LINK_PRIVATE -nodefaultlib -noentry)
  string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
elseif (WIN32 AND ((CMAKE_C_COMPILER_ID MATCHES "GNU") OR (CMAKE_C_COMPILER_ID MATCHES "Clang")))
  # remove stdlib dependency on windows with GCC and Clang (for similar reasons
  # as to MSVC - to allow it to be used with any Windows compiler)
  target_compile_options (glew PRIVATE -fno-builtin -fno-stack-protector)
  target_link_libraries (glew LINK_PRIVATE -nostdlib)
endif ()

target_link_libraries (glew LINK_PUBLIC ${OPENGL_LIBRARIES})

#install (FILES
#    include/GL/wglew.h
#    include/GL/glew.h
#    include/GL/glxew.h
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GL)