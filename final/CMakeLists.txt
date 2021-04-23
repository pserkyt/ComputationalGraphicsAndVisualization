cmake_minimum_required(VERSION 3.15) # 3.15 because need Visual Studio 16 (2019) for Windows
project(final)

set(CMAKE_CXX_STANDARD 14)

# Includes
include_directories(src include include/glm include/stb)
if (APPLE)
    link_directories(/opt/local/lib) # macports
    include_directories(/opt/local/include)
    link_directories(/usr/local/lib) # homebrew
    include_directories(/usr/local/include)
else (APPLE) # Windows
    include_directories(C:/OpenGL/GLEW/include)
    include_directories(C:/OpenGL/GLFW/include)
    include_directories(C:/OpenGL/GLAD)
endif (APPLE)

# OpenGL
if (WIN32)
    find_package(OpenGL REQUIRED)
    message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIR}")
else (WIN32)
    find_library(OPENGL_LIBRARY OpenGL REQUIRED)
    if (OPENGL_LIBRARY)
        message(STATUS "OpenGL library found!")
    else (OPENGL_LIBRARY)
        message("ERROR: OpenGL library NOT found!")
    endif (OPENGL_LIBRARY)
endif (WIN32)

# GLEW
if (WIN32)
    find_library(GLEW_LIBRARY NAMES GLEW GLEW32 PATHS lib/glew/lib/Release/x64 REQUIRED)
    if (GLEW_LIBRARY)
        message(STATUS "GLEW library found!")
    else (GLEW_LIBRARY)
        message("ERROR: GLEW library NOT found!")
    endif (GLEW_LIBRARY)
else (WIN32)
    find_library(GLEW_LIBRARY GLEW REQUIRED)
endif (WIN32)

# GLFW
if (WIN32)
    find_library(GLFW_LIBRARY NAMES GLFW3 PATHS lib/glfw/lib-vc2019 REQUIRED)
    if (GLFW_LIBRARY)
        message(STATUS "GLFW library found!")
    else (GLFW_LIBRARY)
        message("ERROR: GLFW library NOT found!")
    endif (GLFW_LIBRARY)
else (WIN32)
    find_package(GLFW3 REQUIRED)
    message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
    set(GLFW_LIBRARY glfw)
endif (WIN32)

# Libs
set(LIBS ${OPENGL_LIBRARY} ${GLEW_LIBRARY} ${GLFW_LIBRARY})
link_libraries(${LIBS})

set(SOURCES src/camera.cpp src/cube.cpp src/cylinder.cpp src/geometry.cpp src/light.cpp src/main.cpp src/material.cpp
        src/model.cpp src/plane.cpp src/pyramid.cpp src/shader.cpp src/skybox.cpp src/sphere.cpp src/texture.cpp
        src/torus.cpp)

# Sources
add_executable(final ${SOURCES})
