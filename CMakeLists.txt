cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(EXTERNAL_ROOT "${CMAKE_SOURCE_DIR}/external")

project("ParticleSystem")

# Define the base executable target
add_executable(ParticleSystem "App.cpp" "App.h")


set(GLFW_DIR "${EXTERNAL_ROOT}/glfw")
set(GLAD_DIR "${EXTERNAL_ROOT}/glad")
set(GLM_DIR "${EXTERNAL_ROOT}/glm")
set(FREETYPE_DIR "${EXTERNAL_ROOT}/freetype")
set(IMGUI_DIR "${EXTERNAL_ROOT}/imgui")
set(JSON_DIR "${EXTERNAL_ROOT}/json")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ParticleSystem PROPERTY CXX_STANDARD 20)
endif()


add_library(glfw INTERFACE)
target_include_directories(glfw INTERFACE ${GLFW_DIR})
target_link_libraries(glfw INTERFACE ${GLFW_DIR}/lib/glfw3.lib)

add_library(glad INTERFACE)
target_include_directories(glad INTERFACE ${GLAD_DIR}/include)
target_sources(glad INTERFACE ${GLAD_DIR}/src/glad.c)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${GLM_DIR})

add_library(freetype INTERFACE)
target_include_directories(freetype INTERFACE ${FREETYPE_DIR}/include)
target_link_libraries(freetype INTERFACE ${FREETYPE_DIR}/lib/freetype.lib)

add_library(json INTERFACE)
target_include_directories(json INTERFACE ${JSON_DIR})

# Add ImGui as INTERFACE library
add_library(imgui INTERFACE)

# Include ImGui headers for the interface
target_include_directories(imgui INTERFACE
    ${IMGUI_DIR}
)

target_sources(imgui INTERFACE
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui INTERFACE
    glad
    glfw
)

add_custom_command(TARGET ParticleSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${FREETYPE_DIR}/freetype.dll"  # Path to the DLL
    $<TARGET_FILE_DIR:ParticleSystem>  # Output directory of the executable
)

add_subdirectory(src)
#add_subdirectory(external/imgui)

target_include_directories(ParticleSystem PUBLIC
    ${CMAKE_SOURCE_DIR}/external/imgui

    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/opengl
    ${CMAKE_SOURCE_DIR}/src/ParticleEmitters
)


# Link the libraries to the main executable
target_link_libraries(ParticleSystem PUBLIC
    glad
    glfw
    glm
    freetype
    imgui
    json

    src
)


set(ASSETS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSETS_OUTPUT_DIR ${CMAKE_BINARY_DIR}/assets)


add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSETS_SOURCE_DIR} ${ASSETS_OUTPUT_DIR}
    COMMENT "Copying assets directory to build directory"
)

add_dependencies(ParticleSystem CopyAssets)


# Optionally, if you want subprojects to access the same libraries,
# you can link them as INTERFACE libraries in other targets like so:
# target_link_libraries(MySubProject PRIVATE glad glfw)
