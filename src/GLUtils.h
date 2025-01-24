#pragma once

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

#include <glad/glad.h>

namespace GLUtils {
	unsigned int LoadShader(const char* filepath, GLenum shaderType);
}