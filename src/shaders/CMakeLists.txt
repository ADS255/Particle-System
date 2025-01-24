# Find all shader files in the current directory
file(GLOB SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/*")

# custom command to copy the shaders each time the build is run
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_BINARY_DIR}/shaders
    DEPENDS ${SHADERS}
)

# shaders are copied when building
add_dependencies(ParticleSystem copy_shaders)
