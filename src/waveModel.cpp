#include "../include/waveModel.h"

waveModel::waveModel()
{
    oceanographicSpectrum();

    glGenBuffers(1, &specBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, specBuffer);
	glBufferData(GL_ARRAY_BUFFER, geometry.size() * sizeof(GLfloat), &geometry[0], GL_DYNAMIC_DRAW);
}

void waveModel::oceanographicSpectrum()
{
    float alpha = 0.0081f;
    int n = -128;
    int Lx = 1000;
    int Ly = 1000;

    std::default_random_engine generator;
    std::normal_distribution<float> distribution(0.0f, 1.0f);

    float er = distribution(generator);
    float ei = distribution(generator);

    for (int i = n; i < std::abs(n); i++)
    {
        float kx = 2 * M_PI * i / Lx;
        for (int j = n; j < std::abs(n); j++)
        {
            float ky = 2 * M_PI * j / Ly;

            glm::vec2 h0 = spectrumHeight(kx, ky, er, ei);
            glm::vec2 h1 = spectrumHeight(-kx, -ky, er, ei);

            float omega = waveDispersion(kx, ky);

            glm::vec2 spectrum;

            if (omega < 0.00001f)
            {
                spectrum = glm::vec2(0.0f, 0.0f);
            }
            else
            {
                float theta_p = M_PI / 4.0f;

                float w_m = 10.0f / (1.026f * 31.0f);

                float directional = (alpha * 100) / (std::pow(omega, 5));

                float dspectrum = std::exp((-5.0f / 4.0f) * std::pow((w_m / omega), 4)) * std::pow(std::cos(theta_p - std::atan2(ky, kx)), 2);

                float directional_spectrum = directional * dspectrum;

                spectrum = (h0 + h1) * 0.5f * glm::exp(glm::vec2(0.0f, 1.0f) * omega * 0.0f);
            }

            geometry.push_back(spectrum.x);
            geometry.push_back(spectrum.y);
        }
    }
}

glm::vec2 waveModel::spectrumHeight(float kx, float ky, float randr, float randi)
{
    glm::vec2 w = glm::vec2(1.0f, 0.0f);
    glm::vec2 k = glm::vec2(kx, ky);

    float k1 = std::sqrt(std::pow(kx, 2) + std::pow(ky, 2));

    float L = std::pow(31.0f, 2) / 10.0f;

    glm::vec2 h;

    if (k1 < 0.00001f)
    {
        h = glm::vec2(0.0f, 0.0f);
    }
    else
    {
        float L = std::pow(31.0f, 2.0f) / 10.0f;
        glm::vec2 w = glm::vec2(1.0f, 0.0f);

        float p =  1 * std::exp(-1.0f / (k1 * std::pow(L, 2))) / (std::pow(k1, 4)) * std::pow(std::abs(glm::dot(w, k)), 2);

        h = 1.0f / std::sqrt(2.0f) * glm::vec2(randr, randi) * std::sqrt(p);
    }

    return h;
}

float waveModel::waveDispersion(float kx, float ky)
{
    return std::sqrt(10 * std::sqrt(std::pow(kx, 2) + std::pow(ky, 2)));
}
