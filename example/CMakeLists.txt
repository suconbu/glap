project(glapp_example CXX)

add_executable(${PROJECT_NAME} triangle.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<BOOL:${WIN32}>,opengl32,GL> glapp)
