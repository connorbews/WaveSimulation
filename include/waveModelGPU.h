#ifndef WAVEMODELGPU_CLASS_H
#define WAVEMODELGPU_CLASS_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "ComputeShaderClass.h"


class waveModelGPU
{
public:
    waveModelGPU();
	waveModelGPU(int size);
    void wavePropagation(GLuint ID, double dt);

private:
    int n;

    ComputeShader horizontalidft;
    ComputeShader verticalidft;
    ComputeShader wavePropagation;
};

#endif