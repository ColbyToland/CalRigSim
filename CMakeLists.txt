cmake_minimum_required(VERSION 2.8)

# Get project name from top directory
string(REPLACE "/" ";" DIR_LIST ${CMAKE_CURRENT_SOURCE_DIR})
list(REVERSE DIR_LIST)
list(GET DIR_LIST 0 PROJ_NAME)

project(${PROJ_NAME})

SET(CMAKE_CXX_FLAGS "-std=c++0x")

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(${PROJ_NAME} main.cpp
                            glad.c
                            Renderman.cpp
                            CalTarget.cpp
                            CalTex.cpp
                            CalData.cpp
                            Calibrator.cpp)

target_link_libraries(${PROJ_NAME} 
                        ${OpenCV_LIBS} 
                        ${OPENGL_LIBRARIES}
                        dl
                        glfw)
