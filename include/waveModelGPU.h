#ifndef WAVEMODELGPU_CLASS_H
#define WAVEMODELGPU_CLASS_H

#include <glad/glad.h>
#include <cstring>
#include "glm/glm.hpp"
#include "ComputeShaderClass.h"
#include "SSBO.h"


class waveModelGPU
{
public:
    std::vector<GLuint> index;

    waveModelGPU();
	waveModelGPU(int size);

    void updateModel();

private:
    int n;

    ComputeShader initialize;
    ComputeShader horizontalidft;
    ComputeShader verticalidft;
    ComputeShader wavePropagation;
    ComputeShader normalCalculation;
    ComputeShader dt;

    SSBO initializeBuffer;
    SSBO horizontalOutBuffer;
    SSBO verticalOutBuffer;

    void waveInit();
    void waveIDFT();
    void waveNorm();
    void waveIndex();
    void waveProp();
    void waveDt();
};

#endif