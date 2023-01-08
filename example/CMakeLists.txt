project(glapp_example CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

link_libraries($<IF:$<BOOL:${WIN32}>,opengl32,GL> glapp)

add_executable(triangle triangle.cpp)
add_executable(multiwindow multiwindow.cpp)
