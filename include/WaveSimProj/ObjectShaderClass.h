#ifndef OBJECT_SHADER_CLASS_H
#define OBJECT_SHADER_CLASS_H

#include "shaderClass.h"

class ObjectShader : public Shader
{
	public:
        ObjectShader(const char* vertexFile, const char* fragmentFile);
};

#endif