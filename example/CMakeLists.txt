project(glapp_example CXX)

add_executable(${PROJECT_NAME} triangle.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE opengl32 glapp)
