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
#include <mutex>

class waveModel
{
public:
    GLuint specBuffer;
	std::vector<fftw_complex> geometry;
    std::vector<int> index;
    std::vector<double> geometryMesh;

	waveModel();
private:
    void oceanographicSpectrum();
    fftw_complex* spectrumHeight(double kx, double ky, double randr, double randi);
    double waveDispersion(double kx, double ky);
};

#endif