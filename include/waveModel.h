#ifndef WAVEMODEL_CLASS_H
#define WAVEMODEL_CLASS_H

#include "../include/glm/glm.hpp"
#include <glad/glad.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <random>

class waveModel
{
public:
    GLuint specBuffer;
	std::vector<GLfloat> geometry;

	waveModel();
private:
    void oceanographicSpectrum();
    glm::vec2 spectrumHeight(float kx, float ky, float randr, float randi);
    float waveDispersion(float kx, float ky);
};

#endif