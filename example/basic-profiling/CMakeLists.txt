cmake_minimum_required (VERSION 2.6)

project(basic-profiling)

include_directories ("${PROJECT_SOURCE_DIR}/../../include")

add_executable(${PROJECT_NAME} main.cpp foo.cpp bar.cpp)
add_executable(${PROJECT_NAME}-disabled main.cpp foo.cpp bar.cpp)

set_property(
   TARGET "${PROJECT_NAME}"
   PROPERTY COMPILE_DEFINITIONS DYE_ENABLED
)

find_package(Boost COMPONENTS system thread REQUIRED)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

# Enable (and stop on) compiler warnings.
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wall   HAVE_WALL)
check_cxx_compiler_flag(-Werror HAVE_WERROR)
check_cxx_compiler_flag(-Wextra HAVE_WEXTRA)
if (HAVE_WALL)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()
if (HAVE_WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()
if (HAVE_WEXTRA)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
endif()
