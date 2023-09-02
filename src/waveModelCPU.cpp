#include "../include/waveModelCPU.h"

waveModelCPU::waveModelCPU(int size) :
    geometryMesh(6 * std::pow(size, 2), 0)
{
    n = size;

    oceanographicSpectrum();
    initMesh();
    waveIDFT();

    normalsOffset = 3 * std::pow(size, 2);

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

    updateNormals();
}

void waveModelCPU::oceanographicSpectrum()
{
    double alpha = 1;
    int n = -128;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    double er = 0.5;//distribution(generator);
    double ei = 0.5;//distribution(generator);

    for (int i = n; i < std::abs(n); i++)
    {
        double kx = 2.0 * M_PI * i / LX;
        for (int j = n; j < std::abs(n); j++)
        {
            double ky = 2.0 * M_PI * j / LY;

            std::complex<double> h0 = spectrumHeight(kx, ky, er, ei);
            std::complex<double> h1 = spectrumHeight(-kx, -ky, er, ei);

            double omega = waveDispersion(kx, ky);

            std::complex<double> spectrum;

            if (omega == 0.0)
            {
                spectrum = std::complex<double>(0.0, 0.0);
            }
            else
            {
                double theta_p = M_PI / 4.0;

                double w_m = 9.81 / (1.026 * 31.0);

                double directional = (alpha * std::pow(9.81, 2.0)) / (std::pow(omega, 5.0));

                double dspectrum = std::exp((-5.0 / 4.0) * std::pow((w_m / omega), 4.0)) * std::pow(std::cos(theta_p - std::atan2(ky, kx)), 2.0);

                double directional_spectrum = directional * dspectrum;

                spectrum = (h0 + h1) * 0.5 * std::exp(std::complex<double>(0.0, 1.0) * omega * 0.0) * directional_spectrum;
            }

            geometry.push_back(spectrum);
        }
    }
}

void waveModelCPU::waveIDFT()
{
    fftw_complex *in;
    fftw_complex *out;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * geometry.size());
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * geometry.size());

    for (int i = 0 ;  i< geometry.size() ; i++)
    {
        memcpy(&in[i], &geometry[i], sizeof(fftw_complex));
    }

    fftw_plan p = fftw_plan_dft_2d(256, 256,
                in,
                out,
                FFTW_BACKWARD,
                FFTW_ESTIMATE);

    fftw_execute(p);
    fftw_destroy_plan(p);

    updateMesh(out);
}

void waveModelCPU::initMesh()
{
    for (int y = 0; y < n; y++)
    {
        for (int x = 0; x < n; x++)
        {
            geometryMesh[3 * (y * n + x)] = (x * 1000.0f / 256.0f);
            geometryMesh[3 * (y * n + x) + 1] = (y * 1000.0f / 256.0f);
            geometryMesh[3 * (y * n + x) + 2] = 0.0f;
        }
    }
}

void waveModelCPU::updateMesh(fftw_complex* out)
{
    for (int i = 0;  i < geometry.size(); i++)
    {
        GLfloat result = std::sqrt(std::pow(out[i][0] / 65536.0, 2.0) + std::pow(out[i][1] / 65536.0, 2.0));
        geometryMesh[3 * i + 2] = result;
    }

    fftw_free(out);
}

void waveModelCPU::updateNormals()
{
    float dy, dx;
    float upIndex, downIndex, leftIndex, rightIndex;
    const int maxIndex = std::pow(n, 2);
    for (int y = 0; y < n; y++)
    {
        for (int x = 0; x < n; x++)
        {
            upIndex = (y + 1) * n + x;
            downIndex = (y - 1) * n + x;
            leftIndex = y * n + x - 1;
            rightIndex = y * n + x + 1;
            
            dy = calcDy(upIndex, downIndex, maxIndex);
            dx = calcDx(x, y, rightIndex, leftIndex, maxIndex);

            float d = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2) + 1);

            geometryMesh[normalsOffset + 3 * (y * n + x)] = -dx / d;
            geometryMesh[normalsOffset + 3 * (y * n + x) + 1] = -dy / d;
            geometryMesh[normalsOffset + 3 * (y * n + x) + 2] = 1 / d;
        }
    }
}

float waveModelCPU::calcDy(int upIndex, int downIndex, int maxIndex)
{
    float dy;

    if (downIndex < 0)
    {
        dy = 0.5f * (geometryMesh[3 * upIndex + 2] - geometryMesh[3 * (downIndex + maxIndex) + 2]);
    }
    else if (upIndex > maxIndex - 1)
    {
        dy = 0.5f * (geometryMesh[3 * (upIndex - maxIndex) + 2] - geometryMesh[3 * downIndex + 2]);
    }
    else
    {
        dy = 0.5f * (geometryMesh[3 * upIndex + 2] - geometryMesh[3 * downIndex + 2]);
    }

    return dy;
}

float waveModelCPU::calcDx(int x, int y, int rightIndex, int leftIndex, int maxIndex)
{
    float dx;

    if (x - 1 == -1)
    {
        dx = 0.5f * (geometryMesh[3 * rightIndex + 2] - geometryMesh[3 * (y * n + n - 1) + 2]);
    }
    else if (x + 1 == n)
    {
        dx = 0.5f * (geometryMesh[3 * (y * n) + 2] - geometryMesh[3 * leftIndex + 2]);
    }
    else
    {
        dx = 0.5f * (geometryMesh[3 * rightIndex + 2] - geometryMesh[3 * leftIndex + 2]);
    }

    return dx;
}


std::complex<double> waveModelCPU::spectrumHeight(double kx, double ky, double randr, double randi)
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

double waveModelCPU::waveDispersion(double kx, double ky)
{
    return std::sqrt(9.81 * std::sqrt(std::pow(kx, 2) + std::pow(ky, 2)));
}

void waveModelCPU::wavePropagation(GLuint ID, double dt)
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);

    int n = -128;

    for (int x = n; x < abs(n); x++)
    {
        double kx = 2 * M_PI * x / LX;
        for (int y = n; y < abs(n); y++)
        {
            double ky = 2 * M_PI * y / LY;

            std::complex<double> result = geometry[(x + abs(n)) * 256 + y + abs(n)] * std::exp(std::complex<double>(0.0, 1.0) * waveDispersion(kx, ky) * dt);
            geometry[(x + abs(n)) * 256 + y + abs(n)] = result;
        }
    }

    waveIDFT();
    updateNormals();
    glBufferSubData(GL_ARRAY_BUFFER, 0, geometryMesh.size() * sizeof(GLfloat), &geometryMesh[0]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
