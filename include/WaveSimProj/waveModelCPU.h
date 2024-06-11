#ifndef WAVEMODELCPU_CLASS_H
#define WAVEMODELCPU_CLASS_H

#if _WIN32
#define _USE_MATH_DEFINES
#endif

#include "../glad/glad.h"
#include <vector>
#include <cstdlib>
#include <cmath>
#include <random>
#include <complex>
#include "../fftw3/fftw3.h"
#include <iostream>
#include <cstring>
#include <glm/glm.hpp>

#define ALPHA 1
#define LX 1000
#define LY 1000

class waveModelCPU
{
public:
    GLuint specBuffer;
	std::vector<std::complex<double>> complexGeometry;
    std::vector<GLuint> index;
    std::vector<GLfloat> geometry;
    int normalsOffset;

    // Constructor called with no size specified, calling this is an error
    waveModelCPU();

    // Constructor that generates a waveModel of size: "size" x "size" to run on the CPU
	waveModelCPU(int size);

    // Updates each vertex of the waveModel and updates the VBO specified by "ID"
    void updateModel(GLuint ID);

private:
    // Size of waveModel
    int n;

    // Current time, used for wave propagation
    double dt;

    // Initializes a "size" x "size" complex grid in the spectral domain that represents the wave model
    void waveInit();

    // Helper function for waveInit, calculates spectrum height for initialization of complex wave model grid
    std::complex<double> spectrumHeight(double kx, double ky, double randr, double randi);
    // Helper function for waveInit, converts wave number to frequency
    double waveDispersion(double kx, double ky);

    // Converts the complex wave model grid from the spectral domain to the spatial domain
    void waveIDFT();

    // Helper function for waveIDFT, transfers the results from IFFT to the z component of the wave model mesh
    void updateMesh(fftw_complex* out);

    // Calcualtes the normal vectors for each vertex
    void waveNorm();

    // Helper function for waveNorm, calculates the dy and dx variables in waveNorm function
    float calcDy(int x, int y, int maxIndex);
    // Helper function for waveNorm, calculates the dx variables in waveNorm function
    float calcDx(int x, int y, int maxIndex);

    // Initializes the x and y components of the wave model mesh
    void initMesh();

    // Initializes index array for EBO
    void waveIndex();

    // Updates complex wave model grid
    void waveProp();

    // Updates the VBO with new vertex attribute data
    void updateBuffer(GLuint ID);
};

#endif