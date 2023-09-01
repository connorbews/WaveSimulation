#ifndef WAVEMODELCPU_CLASS_H
#define WAVEMODELCPU_CLASS_H

#include "../include/glm/glm.hpp"
#include <glad/glad.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <random>
#include <complex>
#include <fftw3.h>
#include <iostream>
#include <cstring>

class waveModelCPU
{
public:
    GLuint specBuffer;
	std::vector<std::complex<double>> geometry;
    std::vector<GLuint> index;
    std::vector<GLfloat> geometryMesh;
    int normalsOffset;

    waveModelCPU();
	waveModelCPU(int size);
    void wavePropagation(GLuint ID, double dt);
private:
    int n;

    void oceanographicSpectrum();
    void waveIDFT();
    void initMesh();
    void updateMesh(fftw_complex* out);
    void updateNormals();
    std::complex<double> spectrumHeight(double kx, double ky, double randr, double randi);
    double waveDispersion(double kx, double ky);
};

#endif