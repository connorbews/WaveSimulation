#ifndef WAVEMODELGPU_CLASS_H
#define WAVEMODELGPU_CLASS_H

#include <glad/glad.h>
#include <cstring>
#include "ComputeShaderClass.h"
#include "SSBO.h"


class waveModelGPU
{
public:
    std::vector<GLfloat> geometry;
    std::vector<GLuint> index;
    int normalsOffset;

    // Constructor called with no size specified, calling this is an error
    waveModelGPU();

    // Constructor that generates a waveModel of size: "size" x "size" to run on the GPU
	waveModelGPU(int size);

    // Updates each vertex of the waveModel and updates the VBO specified by "ID" 
    void updateModel(GLuint ID);

private:
    // Size of waveModel
    int n;

    // Compute Shader that compiles and links "initCalc.comp" shader
    ComputeShader initCalc;

    // Compute Shader that compiles and links "horizontalIDFT.comp" shader
    ComputeShader horizontalIDFT;

    // Compute Shader that compiles and links "verticalIDFT.comp" shader
    ComputeShader verticalIDFT;

    // Compute Shader that compiles and links "wavePropagation.comp" shader
    ComputeShader wavePropagation;

    // Compute Shader that compiles and links "normalCalc.comp" shader
    ComputeShader normalCalc;

    // Compute Shader that compiles and links "dtCalc.comp" shader
    ComputeShader dtCalc;

    // SSBO to hold wave information in the frequency domain
    SSBO initializeBuffer;

    // SSBO to hold result of horizontal IDFT
    SSBO horizontalOutBuffer;

    // SSBO to hold result of vertical IDFT
    SSBO verticalOutBuffer;

    // Calls the initCalc shader
    void waveInit();

    // Calls the horizontalIDFT and verticalIDFT shader's
    void waveIDFT();

    // Calls the normalCalc shader
    void waveNorm();

    // Initializes index array for EBO
    void waveIndex();

    // Calls the wavePropagation shader
    void waveProp();

    // Calls the dtCalc shader
    void waveDt();

    // Transfers the position and normal information from GPU to CPU to update the VBO
    void getPositionDataGPU();

    // Updates the VBO with new vertex attribute data
    void updateBuffer(GLuint ID);
};

#endif