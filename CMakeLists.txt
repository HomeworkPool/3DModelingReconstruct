cmake_minimum_required(VERSION 3.23)
project(CppAlgoEnv)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR src)
set(EXECUTABLE_OUTPUT_PATH bin/x64)

# add_executable(CppAlgoEnv main.cpp)

########### OpenGL GLUT ############
# Includes
include_directories(include)
link_directories(lib/x64)

########### Project Source Settings ##############
# Auto import all source files in the src directory
file(GLOB_RECURSE files src/*.cpp src/*.c)
foreach (file ${files})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    add_executable (${exe} ${file})

    ########### Link libraries ############
    # OpenGL GLUT
    target_link_libraries(${exe} libfreeglut.a libfreeglut_static.a opengl32 glu32)

    message (\ \ \ \ --\ ${file}\ will\ be\ compiled\ to\ bin/${exe})
endforeach ()

