project(glapp_example CXX)

link_libraries($<IF:$<BOOL:${WIN32}>,opengl32,GL> glapp)

add_executable(triangle triangle.cpp)
add_executable(multiwindow multiwindow.cpp)
