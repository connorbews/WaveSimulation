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
    std::vector<GLfloat> geometry;
    std::vector<GLuint> index;
    int normalsOffset;

    waveModelGPU();
	waveModelGPU(int size);

    void updateModel(GLuint ID);

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
    void geometryMapInit();
    void geometryMap(GLuint ID);
};

#endif