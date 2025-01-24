add_library(src STATIC
    "GLUtils.h"
    "GLUtils.cpp"
)

target_include_directories(src PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(src PRIVATE
    glad
    glfw
)

add_subdirectory(shaders)