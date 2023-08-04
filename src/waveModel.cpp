#include "../include/waveModel.h"

waveModel::waveModel()
{
    fftw_complex *in;
    double *out;

    oceanographicSpectrum();

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * geometry.size());
    out = (double*) fftw_malloc(sizeof(double) * geometry.size());

    for (int i = 0 ;  i< geometry.size() ; i++)
    {
       memcpy(&in[i], &geometry[i], sizeof(fftw_complex));
    }

    fftw_plan p = fftw_plan_dft_c2r_2d(256, 256,
                in,
                out,
                FFTW_ESTIMATE);

    fftw_execute(p);
    fftw_destroy_plan(p);

    for (int i = 0;  i < geometry.size(); i++)
    {
        geometryMesh.push_back((i % 256) * 1000.0f / 256.0f);
        geometryMesh.push_back((i / 256) * 1000.0f / 256.0f);
        geometryMesh.push_back(float(out[i]));
    }

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
 }

void waveModel::oceanographicSpectrum()
{
    double alpha = 0.0081;
    int n = -128;
    int Lx = 1000;
    int Ly = 1000;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    double er = distribution(generator);
    double ei = distribution(generator);

    for (int i = n; i < std::abs(n); i++)
    {
        double kx = 2.0 * M_PI * i / Lx;
        for (int j = n; j < std::abs(n); j++)
        {
            double ky = 2.0 * M_PI * j / Ly;

            std::complex<double> h0 = spectrumHeight(kx, ky, er, ei);
            std::complex<double> h1 = spectrumHeight(-kx, -ky, er, ei);

            double omega = waveDispersion(kx, ky);

            std::complex<double> spectrum;

            if (omega < 0.00001f)
            {
                spectrum = std::complex<double>(0.0, 0.0);
            }
            else
            {
                double theta_p = M_PI / 4.0;

                double w_m = 10.0 / (1.026 * 31.0);

                double directional = (alpha * 100.0) / (std::pow(omega, 5));

                double dspectrum = std::exp((-5.0f / 4.0f) * std::pow((w_m / omega), 4)) * std::pow(std::cos(theta_p - std::atan2(ky, kx)), 2);

                double directional_spectrum = directional * dspectrum;

                spectrum = (h0 + h1) * 0.5 * std::exp(std::complex<double>(0.0, 1.0) * omega * 0.0);
            }

            geometry.push_back(spectrum);
        }
    }
}

std::complex<double> waveModel::spectrumHeight(double kx, double ky, double randr, double randi)
{
    glm::vec2 w = glm::vec2(1.0f, 0.0f);
    glm::vec2 k = glm::vec2(kx, ky);

    double k1 = std::sqrt(std::pow(kx, 2) + std::pow(ky, 2));

    double L = std::pow(31.0, 2) / 10.0;

    std::complex<double> h;

    if (k1 < 0.00001f)
    {
        h = std::complex<double>(0.0, 0.0);
    }
    else
    {
        glm::vec2 w = glm::vec2(1.0f, 0.0f);

        double p =  1.0 * std::exp(-1.0 / (k1 * std::pow(L, 2))) / (std::pow(k1, 4)) * std::pow(std::abs(glm::dot(w, k)), 2);

        h = 1.0 / std::sqrt(2.0) * std::complex<double>(randr, randi) * std::sqrt(p);
    }

    return h;
}

double waveModel::waveDispersion(double kx, double ky)
{
    return std::sqrt(10 * std::sqrt(std::pow(kx, 2) + std::pow(ky, 2)));
}
