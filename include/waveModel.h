#ifndef WAVEMODEL_CLASS_H
#define WAVEMODEL_CLASS_H

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

class waveModel
{
public:
    GLuint specBuffer;
	std::vector<std::complex<double>> geometry;
    std::vector<GLuint> index;
    std::vector<GLfloat> geometryMesh;

	waveModel();
private:
    void oceanographicSpectrum();
    std::complex<double> spectrumHeight(double kx, double ky, double randr, double randi);
    double waveDispersion(double kx, double ky);
};

#endif