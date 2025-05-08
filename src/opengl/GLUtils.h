#pragma once

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

#include <glad/glad.h>

namespace GLUtils {

	void APIENTRY OpenGlDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	unsigned int LoadShader(const char* filepath, GLenum shaderType);

	void Info();
}