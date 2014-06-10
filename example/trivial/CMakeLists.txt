cmake_minimum_required (VERSION 2.6)

project(trivial)

include_directories ("${PROJECT_SOURCE_DIR}/../../include")

add_executable(${PROJECT_NAME} "${PROJECT_NAME}.cpp")

#find_package(Boost COMPONENTS program_options REQUIRED)

#target_link_libraries(${PROJECT_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY})

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

# Enable compiler-assisted instrumentation.
check_cxx_compiler_flag(-finstrument-functions HAVE_FINSTRUMENTFUNCTIONS)
if (HAVE_FINSTRUMENTFUNCTIONS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finstrument-functions")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finstrument-functions-exclude-file-list=/bits/,/include/dye/")
    target_link_libraries(${PROJECT_NAME} -ldl)
endif()
