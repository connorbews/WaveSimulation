#include "../../include/WaveSimProj/waveModelCPU.h"

waveModelCPU::waveModelCPU()
{
    std::cerr << "No size specified" << std::endl;
}

waveModelCPU::waveModelCPU(int size) :
    geometry(6 * std::pow(size, 2), 0)
{
    n = size;
    normalsOffset = 3 * std::pow(size, 2);
    dt = 0.0;

    waveIndex();
    waveInit();
    initMesh();
    AllocateFFTWResources();
    waveIFFT();
    waveNorm();

    
}
waveModelCPU::~waveModelCPU()
{
    ReleaseFFTWResources();
}

void waveModelCPU::updateModel(GLuint ID)
{
    waveProp();
    waveIFFT();
    waveNorm();
    updateBuffer(ID);
    dt += 1.0 / 500.0;
}

void waveModelCPU::waveInit()
{
    int minLimit = -n / 2;
    int maxLimit = n / 2;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    double er = distribution(generator);
    double ei = distribution(generator);

    for (int i = minLimit; i < maxLimit; i++)
    {
        double kx = 2.0 * M_PI * i / LX;
        for (int j = minLimit; j < maxLimit; j++)
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
                double directional = (ALPHA * std::pow(9.81, 2.0)) / (std::pow(omega, 5.0));
                double dspectrum = std::exp((-5.0 / 4.0) * std::pow((w_m / omega), 4.0)) * std::pow(std::cos(theta_p - std::atan2(ky, kx)), 2.0);
                double directional_spectrum = directional * dspectrum;

                spectrum = (h0 + h1) * 0.5 * std::exp(std::complex<double>(0.0, 1.0) * omega * 0.0) * directional_spectrum;
            }

            complexGeometry.push_back(spectrum);
        }
    }
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

void waveModelCPU::AllocateFFTWResources()
{
    ComplexWave = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * complexGeometry.size());
    RealWave = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * complexGeometry.size());

    WaveIFFTPlan = fftw_plan_dft_2d(n, n, ComplexWave, RealWave, FFTW_BACKWARD, FFTW_ESTIMATE);
}

void waveModelCPU::ReleaseFFTWResources()
{
    fftw_destroy_plan(WaveIFFTPlan);

    fftw_free(ComplexWave);
    fftw_free(RealWave);
}

void waveModelCPU::waveIFFT()
{
    for (int i = 0 ;  i < complexGeometry.size() ; i++)
    {
        memcpy(&ComplexWave[i], &complexGeometry[i], sizeof(fftw_complex));
    }

    fftw_execute(WaveIFFTPlan);
    updateMesh(RealWave);
}

void waveModelCPU::updateMesh(fftw_complex* out)
{
    for (int i = 0;  i < complexGeometry.size(); i++)
    {
        GLfloat result = std::sqrt(std::pow(out[i][0] / 65536.0, 2.0) + std::pow(out[i][1] / 65536.0, 2.0));
        geometry[3 * i + 2] = result;
    }
}

void waveModelCPU::waveNorm()
{
    float dy, dx;
    const int maxIndex = std::pow(n, 2);
    for (int y = 0; y < n; y++)
    {
        for (int x = 0; x < n; x++)
        {
            dy = calcDy(x, y, maxIndex);
            dx = calcDx(x, y, maxIndex);

            float d = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2) + 1);

            geometry[normalsOffset + 3 * (y * n + x)] = -dx / d;
            geometry[normalsOffset + 3 * (y * n + x) + 1] = -dy / d;
            geometry[normalsOffset + 3 * (y * n + x) + 2] = 1 / d;
        }
    }
}

float waveModelCPU::calcDy(int x, int y, int maxIndex)
{
    float dy;
    int upIndex = (y + 1) * n + x;
    int downIndex = (y - 1) * n + x;

    if (downIndex < 0)
    {
        dy = 0.5f * (geometry[3 * upIndex + 2] - geometry[3 * (downIndex + maxIndex) + 2]);
    }
    else if (upIndex > maxIndex - 1)
    {
        dy = 0.5f * (geometry[3 * (upIndex - maxIndex) + 2] - geometry[3 * downIndex + 2]);
    }
    else
    {
        dy = 0.5f * (geometry[3 * upIndex + 2] - geometry[3 * downIndex + 2]);
    }

    return dy;
}

float waveModelCPU::calcDx(int x, int y, int maxIndex)
{
    float dx;
    int leftIndex = y * n + x - 1;
    int rightIndex = y * n + x + 1;

    if (x - 1 == -1)
    {
        dx = 0.5f * (geometry[3 * rightIndex + 2] - geometry[3 * (y * n + n - 1) + 2]);
    }
    else if (x + 1 == n)
    {
        dx = 0.5f * (geometry[3 * (y * n) + 2] - geometry[3 * leftIndex + 2]);
    }
    else
    {
        dx = 0.5f * (geometry[3 * rightIndex + 2] - geometry[3 * leftIndex + 2]);
    }

    return dx;
}

void waveModelCPU::initMesh()
{
    for (int y = 0; y < n; y++)
    {
        for (int x = 0; x < n; x++)
        {
            geometry[3 * (y * n + x)] = (x * 1000.0f / 256.0f);
            geometry[3 * (y * n + x) + 1] = (y * 1000.0f / 256.0f);
            geometry[3 * (y * n + x) + 2] = 0.0f;
        }
    }
}

void waveModelCPU::waveIndex()
{
	for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            index.push_back(i * n + j);
            index.push_back((i + 1) * n + j);
            index.push_back(i * n + j + 1);

            index.push_back((i + 1) * n + j + 1);
            index.push_back(i * n + j + 1);
            index.push_back((i + 1) * n + j);
        }
    }
}

void waveModelCPU::waveProp()
{
    int minLimit = -n / 2;
    int maxLimit = n / 2;

    for (int x = minLimit; x < maxLimit; x++)
    {
        double kx = 2 * M_PI * x / LX;
        for (int y = minLimit; y < maxLimit; y++)
        {
            double ky = 2 * M_PI * y / LY;

            std::complex<double> imaginary = std::complex<double>(0.0, 1.0);

            double omega = waveDispersion(kx, ky);

            std::complex<double> complexAmp1 = complexGeometry[(x + maxLimit) * n + y + maxLimit];

            int negx = int((-1.0 * kx * LX) / (2 * M_PI));
            int negy = int((-1.0 * ky * LY) / (2 * M_PI));

            std::complex<double> complexAmp2 = complexGeometry[(negx + maxLimit - 1) * n + negy + maxLimit - 1];

            std::complex<double> result = complexAmp1 * std::exp(imaginary * omega * dt);
            complexGeometry[(x + maxLimit) * n + y + maxLimit] = result;
        }
    }
}

void waveModelCPU::updateBuffer(GLuint ID)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, geometry.size() * sizeof(GLfloat), &geometry[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
