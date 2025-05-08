#include "GLUtils.h"

unsigned int GLUtils::LoadShader(const char* filepath, GLenum shaderType) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return 0;
    }

    std::stringstream shaderStream;
    shaderStream << file.rdbuf();
    std::string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();

    unsigned int shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {

        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        char* message = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, message);
        std::cerr << "Shader compilation error in " << filepath << ":\n" << message << std::endl;

        delete[] message;
    }

    return shader;
}

void GLUtils::Info()
{
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Get the maximum number of workgroups (X, Y, Z)
	int maxWorkGroups[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &maxWorkGroups[0]); // X dimension
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &maxWorkGroups[1]); // Y dimension
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &maxWorkGroups[2]); // Z dimension

	std::cout << "Max Workgroups: "
		<< "X: " << maxWorkGroups[0] << ", "
		<< "Y: " << maxWorkGroups[1] << ", "
		<< "Z: " << maxWorkGroups[2] << std::endl;

	int maxLocalSizeDimensions[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxLocalSizeDimensions[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxLocalSizeDimensions[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxLocalSizeDimensions[2]);

	std::cout << "Max Local Dimensions: "
		<< "X: " << maxLocalSizeDimensions[0] << ", "
		<< "Y: " << maxLocalSizeDimensions[1] << ", "
		<< "Z: " << maxLocalSizeDimensions[2] << std::endl;

	// Query the maximum number of SSBO bindings
	GLint maxSSBOs;
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &maxSSBOs);
	std::cout << "Maximum number of SSBO bindings: " << maxSSBOs << std::endl;

	// Query the maximum size of an individual SSBO
	GLint maxSSBOSize;
	glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSSBOSize);
	std::cout << "Maximum size of an individual SSBO: " << maxSSBOSize << " bytes" << std::endl;
}


void APIENTRY GLUtils::OpenGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	std::string src;
	switch (source) {
	case GL_DEBUG_SOURCE_API: src = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: src = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: src = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: src = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: src = "Other"; break;
	}

	std::string typ;
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: typ = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typ = "Deprecated Behavior"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typ = "Undefined Behavior"; break;
	case GL_DEBUG_TYPE_PORTABILITY: typ = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: typ = "Performance"; break;
	case GL_DEBUG_TYPE_OTHER: typ = "Other"; break;
	case GL_DEBUG_TYPE_MARKER: typ = "Marker"; break;
	}

	std::string sev;
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: sev = "High"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: sev = "Medium"; break;
	case GL_DEBUG_SEVERITY_LOW: sev = "Low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: sev = "Notification"; break;
	}

	std::cout << "OpenGL Error [" << id << "]: " << message << std::endl;
	std::cout << " Source: " << src << std::endl;
	std::cout << " Type: " << typ << std::endl;
	std::cout << " Severity: " << sev << "\n" << std::endl;

	std::exit(EXIT_FAILURE);
}