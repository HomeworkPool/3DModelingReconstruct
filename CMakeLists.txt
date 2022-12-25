cmake_minimum_required(VERSION 3.0)

# VCPKG Support
# 由于ENV_VCPKG_ROOT环境变量可能未设置，因此需要检测并发出警告
# CMAKE_TOOLCHAIN_FILE 需要在project()之前设置
IF(DEFINED ENV{VCPKG_ROOT})
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" ENV_VCPKG_ROOT)
    message ("--\ Enabling VCPKG support: ${ENV_VCPKG_ROOT}")
    set(CMAKE_TOOLCHAIN_FILE "${ENV_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
ELSE()
    MESSAGE(WARNING "VCpkg not found. Please install vcpkg and set the environment variable VCPKG_ROOT")
ENDIF()

########### Begin Project ###########
project(CppAlgoEnv)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_SOURCE_DIR src)
set(EXECUTABLE_OUTPUT_PATH bin/x64)
set(LIBRARY_OUTPUT_PATH lib/x64)

# add_executable(CppAlgoEnv main.cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wa,-mbig-obj")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

########### Local packages ############
# Includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
link_directories(lib/x64 ${CMAKE_CURRENT_BINARY_DIR})
include(${CMAKE_CURRENT_SOURCE_DIR}/packages.cmake)

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


#########


########### Project Source Settings ##############
message ("\ --\ import all MAIN Entry source files in ${CMAKE_CURRENT_SOURCE_DIR}/src/main")

file(GLOB_RECURSE files ${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/main/*.c)
foreach (file ${files})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    add_executable (${exe} ${file})

    ########### Link libraries ############
    target_link_libraries(${exe} CppAlgoEnvLibs)
    include(${CMAKE_CURRENT_SOURCE_DIR}/target_link_libraries.cmake)

    message (\ \ \ \ --\ ${file}\ will\ be\ compiled\ to\ bin/${exe})
endforeach ()

########### Link all resources ############
get_filename_component(buildDirRelFilePath "${CMAKE_BINARY_DIR}"
        REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
file(RELATIVE_PATH buildDirRelFilePath "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

message ("--\ Execute: Linking all resources ")
#message ("\ --\ Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
#message ("\ --\ Current output dir: ${CMAKE_BINARY_DIR}")
execute_process(COMMAND cmd /C PreBuild.bat ${buildDirRelFilePath} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
