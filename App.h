#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ft2build.h>
#include <freetype/freetype.h>

#include <json.hpp>


#include "src/opengl/GLUtils.h"
#include "src/opengl/VertexArrayObject.h"
#include "src/opengl/VertexBufferObject.h"
#include "src/ParticleEmitters/Particle.h"

#include "src/ParticleEmitters/BaseParticleEmitter.h"
#include "src/ParticleEmitters/BaseParticleEmitter_3.h"

#include "src/benchmarking/PerformanceProfiler.h"

