cmake_minimum_required(VERSION 3.11)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(GLAPP_STANDALONE TRUE)
endif()

option(GLAPP_BUILD_EXAMPLES "Build the examples" ${GLAPP_STANDALONE})
option(GLAPP_BUILD_TESTS "Build the test programs" ${GLAPP_STANDALONE})

set(CMAKE_CXX_STANDARD 11)

project(glapp CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE glfw)

add_subdirectory(include)
if(GLAPP_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
if(GLAPP_BUILD_TESTS)
    add_subdirectory(test)
endif()

find_package(GLFW QUIET)
if(NOT GLFW_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
    )
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    FetchContent_MakeAvailable(glfw)
endif()
