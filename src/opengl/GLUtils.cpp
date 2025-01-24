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
