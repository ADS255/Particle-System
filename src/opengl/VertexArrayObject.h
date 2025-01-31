#pragma once

#include<glad/glad.h>
#include "VertexBufferObject.h"

class VertexArrayObject
{
public:
	GLuint ID;

	VertexArrayObject();

	void LinkVertexBufferObject(VertexBufferObject VBO, GLuint layout);
	void LinkAttribute(VertexBufferObject& VBO, GLuint layout,GLuint numComponents,GLenum type,GLboolean normalised,GLsizeiptr stride,void* offset);
	void Bind();
	void Unbind();
	void Delete();

private:

};