cmake_minimum_required(VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(EXTERNAL_ROOT "${CMAKE_SOURCE_DIR}/external")

project("ParticleSystem")

add_executable(ParticleSystem "App.cpp" "App.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ParticleSystem PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(ParticleSystem PRIVATE
    ${EXTERNAL_ROOT}/glfw/include
    ${EXTERNAL_ROOT}/glad/include
)

target_link_libraries(ParticleSystem PRIVATE
    ${EXTERNAL_ROOT}/glfw/lib/glfw3.lib
)

target_sources(ParticleSystem PRIVATE 
    ${EXTERNAL_ROOT}/glad/src/glad.c
)
