add_library(src STATIC
   "opengl/GLUtils.h"
   "opengl/GLUtils.cpp"
   "opengl/VertexArrayObject.cpp" 
   "opengl/VertexArrayObject.h"
   "opengl/VertexBufferObject.cpp"
   "opengl/VertexBufferObject.h" 
 "ParticleEmitters/ParticleEmitter.h" "ParticleEmitters/ParticleEmitter.cpp" "ParticleEmitters/Particle.h" "ParticleEmitters/BaseParticleEmitter.h" "ParticleEmitters/BaseParticleEmitter.cpp"  "ParticleEmitters/ParticlePropertyModifier.h"   "ParticleEmitters/ColourModifier.h")

target_include_directories(src PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/opengl
    ${CMAKE_SOURCE_DIR}/src/ParticleEmitters
)


target_link_libraries(src PRIVATE
    glad
    glfw
    glm
    freetype
    imgui
    json
)

add_subdirectory(shaders)