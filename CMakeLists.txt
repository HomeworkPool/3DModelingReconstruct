cmake_minimum_required(VERSION 3.23)
project(CppAlgoEnv)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR src)
set(EXECUTABLE_OUTPUT_PATH bin/x64)
set(LIBRARY_OUTPUT_PATH lib/x64)

# add_executable(CppAlgoEnv main.cpp)

########### OpenGL GLUT ############
# Includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
link_directories(lib/x64 ${CMAKE_CURRENT_BINARY_DIR})

########### Project Source Settings ##############
# Auto import all source files in the src directory
message ("--\ Auto import all source files in the src directory")

message ("\ --\ import all Library source files in ${CMAKE_CURRENT_SOURCE_DIR}/src/lib")
file(GLOB LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.c)
file(GLOB LIB_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.hpp ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*.h)
add_library(CppAlgoEnvLibs ${LIB_SOURCES} ${LIB_HEADERS} )
INSTALL(TARGETS CppAlgoEnvLibs)
# 为dll设置linker
# 指定dll的生成目录，这里是：./lib_out
SET_TARGET_PROPERTIES(CppAlgoEnvLibs PROPERTIES LINKER_LANGUAGE C
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
        OUTPUT_NAME "CppAlgoEnvLibs"
        PREFIX "")

message ("\ --\ import all MAIN Entry source files in ${CMAKE_CURRENT_SOURCE_DIR}/src/main")

file(GLOB_RECURSE files ${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.c)
foreach (file ${files})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    add_executable (${exe} ${file})

    ########### Link libraries ############
    # OpenGL GLUT
    target_link_libraries(${exe} CppAlgoEnvLibs libfreeglut.a libfreeglut_static.a opengl32 glu32)

    message (\ \ \ \ --\ ${file}\ will\ be\ compiled\ to\ bin/${exe})
endforeach ()

########### Link all resources ############
message ("--\ Execute: Linking all resources ")
execute_process(COMMAND cmd /C PreBuild.bat WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
