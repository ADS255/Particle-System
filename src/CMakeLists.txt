add_library(src STATIC
   "opengl/GLUtils.h"
   "opengl/GLUtils.cpp"
   "opengl/VertexArrayObject.cpp" 
   "opengl/VertexArrayObject.h"
   "opengl/VertexBufferObject.cpp"
   "opengl/VertexBufferObject.h" 
)

target_include_directories(src PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(src PRIVATE
    glad
    glfw
)

add_subdirectory(shaders)