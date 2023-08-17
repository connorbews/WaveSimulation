#ifndef WAVEMODELGPU_CLASS_H
#define WAVEMODELGPU_CLASS_H

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "ComputeShaderClass.h"


class waveModelGPU
{
public:
    std::vector<GLfloat> geometry;
    std::vector<GLuint> index;
    waveModelGPU();
	waveModelGPU(int size);
    void waveProp(GLuint ID, double dt);

private:
    int n;
};

#endif