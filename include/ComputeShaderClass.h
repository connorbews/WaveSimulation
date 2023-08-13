#ifndef COMPUTE_SHADER_CLASS_H
#define COMPUTE_SHADER_CLASS_H

#include "shaderClass.h"

class ComputeShader : public Shader
{
	public:
		ComputeShader(const char* computeFile);
};

#endif