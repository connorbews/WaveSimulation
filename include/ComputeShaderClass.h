#ifndef COMPUTE_SHADER_CLASS_H
#define COMPUTE_SHADER_CLASS_H

#include "shaderClass.h"

class ComputeShader : public Shader
{
	public:
		ComputeShader();
		ComputeShader(const char* computeFile);

        void Activate(int workGroupx, int workGroupy, int workGroupz);
};

#endif