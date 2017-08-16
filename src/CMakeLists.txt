SET(CMAKE_CXX_FLAGS "-std=c++0x -D__DEBUG_COUT__")

find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(${PROJ_NAME} main.cpp
                            glad.c
                            CalTarget.cpp
                            CalTex.cpp
                            CalData.cpp
                            Calibrator.cpp
                            CameraModel.cpp
                            CapturePositionConfig.cpp
                            ConfigParser.cpp
                            GeomTransform.cpp
                            Renderman.cpp
                            TextureConfigData.cpp)

target_link_libraries(${PROJ_NAME} 
                        ${OpenCV_LIBS} 
                        ${OPENGL_LIBRARIES}
                        dl
                        glfw)
