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
	std::vector<glm::vec2> geometry;

	waveModel();
private:
    void oceanographicSpectrum();
    glm::vec2 spectrumHeight(float kx, float ky);
    float waveDispersion(float kx, float ky);
};
#endif