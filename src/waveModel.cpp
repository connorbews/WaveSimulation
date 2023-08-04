#include "../include/waveModel.h"

waveModel::waveModel()
{
    oceanographicSpectrum();

    std::cout << "geometry size: " << geometry.size() << std::endl;

    fftw_plan p = fftw_plan_dft_c2r_2d(256, 256,
                &geometry[0],
                &geometryMesh[0],
                FFTW_ESTIMATE);

    std::cout << "got here: " << std::endl;

    fftw_execute(p);
    fftw_destroy_plan(p);

    for (int i = 0; i < 256 - 1; i++)
    {
        for (int j = 0; j < 256 - 1; j++)
        {
            index.push_back(i * 256 + j);
            index.push_back((i + 1) * 256 + j);
            index.push_back(i * 256 + j + 1);

            index.push_back((i + 1) * 256 + j + 1);
            index.push_back(i * 256 + j + 1);
            index.push_back((i + 1) * 256 + j);
        }
    }

    std::cout << "index size: " << index.size() << std::endl;
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

            fftw_complex* h0 = spectrumHeight(kx, ky, er, ei);
            fftw_complex* h1 = spectrumHeight(-kx, -ky, er, ei);

            double omega = waveDispersion(kx, ky);

            fftw_complex spectrum;

            if (omega < 0.00001f)
            {
                spectrum[0] = 0.0;
                spectrum[1] = 0.0;
            }
            else
            {
                float theta_p = M_PI / 4.0f;

                float w_m = 10.0f / (1.026f * 31.0f);

                float directional = (alpha * 100) / (std::pow(omega, 5));

                float dspectrum = std::exp((-5.0f / 4.0f) * std::pow((w_m / omega), 4)) * std::pow(std::cos(theta_p - std::atan2(ky, kx)), 2);

                float directional_spectrum = directional * dspectrum;

                spectrum[0] = (*h0[0] + *h1[0]) * 0.5 * 1.0;
                spectrum[1] = (*h0[1] + *h1[1]) * 0.5 * 1.0;
            }

            geometry.push_back(spectrum);
        }
    }
}

fftw_complex* waveModel::spectrumHeight(double kx, double ky, double randr, double randi)
{
    glm::vec2 w = glm::vec2(1.0f, 0.0f);
    glm::vec2 k = glm::vec2(kx, ky);

    double k1 = std::sqrt(std::pow(kx, 2) + std::pow(ky, 2));

    double L = std::pow(31.0, 2) / 10.0;

    fftw_complex h;

    if (k1 < 0.00001f)
    {
        h[0] = 0.0;
        h[1] = 0.0;
    }
    else
    {
        glm::vec2 w = glm::vec2(1.0f, 0.0f);

        double p =  1.0 * std::exp(-1.0 / (k1 * std::pow(L, 2))) / (std::pow(k1, 4)) * std::pow(std::abs(glm::dot(w, k)), 2);

        h[0] = 1.0 / std::sqrt(2.0) * randr * std::sqrt(p);
        h[1] = 1.0 / std::sqrt(2.0) * randr * std::sqrt(p);
    }

    return &h;
}

double waveModel::waveDispersion(double kx, double ky)
{
    return std::sqrt(10 * std::sqrt(std::pow(kx, 2) + std::pow(ky, 2)));
}
