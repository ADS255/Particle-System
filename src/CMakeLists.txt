add_library(src STATIC
   "opengl/GLUtils.h"
   "opengl/GLUtils.cpp"
   "opengl/VertexArrayObject.cpp" 
   "opengl/VertexArrayObject.h"
   "opengl/VertexBufferObject.cpp"
   "opengl/VertexBufferObject.h" 
 "ParticleEmitters/ParticleEmitter.h" 
 "ParticleEmitters/ParticleEmitter.cpp" 
 "ParticleEmitters/Particle.h" 
 "ParticleEmitters/BaseParticleEmitter.h" 
 "ParticleEmitters/BaseParticleEmitter.cpp"  
 "ParticleEmitters/ParticlePropertyModifier.h"   
  
  
 "ParticleEmitters/EmitterProperties.h" 
 "ParticleEmitters/VelocityModifier.h" 
 "ParticleEmitters/VelocityModifier.cpp" 
 "ParticleEmitters/IParticlePropertyModifier.h"
 "ParticleEmitters/ColourModifier.h"
"ParticleEmitters/ColourModifier.cpp"
 "ParticleEmitters/PositionModifier.h" 
 "ParticleEmitters/PositionModifier.cpp" 
 "ParticleEmitters/FloatModifier.h" 
 "ParticleEmitters/FloatModifier.cpp" 
 "ParticleEmitters/LifetimeModifier.h"
 "ParticleEmitters/LifetimeModifier.cpp" 
 "ParticleEmitters/Vec3Modifier.h" 
 "ParticleEmitters/Vec3Modifier.cpp"
 "ParticleEmitters/SizeModifier.h"
 "ParticleEmitters/SizeModifier.cpp"
 "benchmarking/PerformanceProfiler.h" "benchmarking/PerformanceMetrics.h" "benchmarking/PerformanceProfiler.cpp")

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
    stb
)

add_subdirectory(shaders)