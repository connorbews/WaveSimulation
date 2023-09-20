#include "../../include/WaveSimProj/waveModelCPU.h"

// Constructor called with no size specified, calling this is an error
waveModelCPU::waveModelCPU()
{
    std::cerr << "No size specified" << std::endl;
}

// Constructor that generates a waveModel of size: "size" x "size" to run on the CPU
waveModelCPU::waveModelCPU(int size) :
    geometry(6 * std::pow(size, 2), 0)
{
    n = size;
    normalsOffset = 3 * std::pow(size, 2);
    dt = 0.0;

    waveIndex();
    waveInit();
    initMesh();
    waveIDFT();
    waveNorm();
}

// Updates each vertex of the waveModel and updates the VBO specified by "ID"
void waveModelCPU::updateModel(GLuint ID)
{
    waveProp();
    waveIDFT();
    waveNorm();
    updateBuffer(ID);
    dt += 1.0 / 500.0;
}

// Initializes a "size" x "size" complex grid in the spectral domain that represents the wave model
void waveModelCPU::waveInit()
{
    int minLimit = -n / 2;
    int maxLimit = n / 2;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);

    double er = 0.5;//distribution(generator);
    double ei = 0.5;//distribution(generator);

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
                double directional = (ALPHA * std::pow(GRAVITY, 2.0)) / (std::pow(omega, 5.0));
                double dspectrum = std::exp((-5.0 / 4.0) * std::pow((w_m / omega), 4.0)) * std::pow(std::cos(theta_p - std::atan2(ky, kx)), 2.0);
                double directional_spectrum = directional * dspectrum;

                spectrum = (h0 + h1) * 0.5 * std::exp(std::complex<double>(0.0, 1.0) * omega * 0.0) * directional_spectrum;
            }

            std::complex<double> imaginary = std::complex<double>(0.0, 1.0);
            double k = std::sqrt(std::pow(kx, 2.0) + std::pow(ky, 2.0));

            complexGeometryZ.push_back(spectrum);
            
            if (k <= 0.000001)
            {
                complexGeometryX.push_back(std::complex<double>(0.0, 0.0));
                complexGeometryY.push_back(std::complex<double>(0.0, 0.0));
            }
            else
            {
                //if (i == minLimit)
                //    std::cout << "start: " << (imaginary * kx * spectrum) / k << std::endl;
                complexGeometryX.push_back((imaginary * kx * spectrum) / k);
                complexGeometryY.push_back((imaginary * ky * spectrum) / k);
            }
        }
    }
}

// Helper function for waveInit, calculates spectrum height for initialization of complex wave model grid
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

// Helper function for waveInit, converts wave number to frequency
double waveModelCPU::waveDispersion(double kx, double ky)
{
    return std::sqrt(GRAVITY * std::sqrt(std::pow(kx, 2.0) + std::pow(ky, 2.0)));
}

// Converts the complex wave model grid from the spectral domain to the spatial domain
void waveModelCPU::waveIDFT()
{
    fftw_complex *zin;
    fftw_complex *zout;

    zin = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * complexGeometryZ.size());
    zout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * complexGeometryZ.size());

    for (int i = 0 ;  i < complexGeometryZ.size() ; i++)
    {
        memcpy(&zin[i], &complexGeometryZ[i], sizeof(fftw_complex));
    }

    fftw_plan zp = fftw_plan_dft_2d(256, 256,
                zin,
                zout,
                FFTW_BACKWARD,
                FFTW_ESTIMATE);

    fftw_execute(zp);
    
    fftw_complex *xin;
    fftw_complex *xout;

    xin = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * complexGeometryX.size());
    xout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * complexGeometryX.size());

    for (int i = 0;  i < complexGeometryX.size() ; i++)
    {
        /*if (i == 1)
        {
            std::cout << complexGeometryX[i] << std::endl;
        }*/
        memcpy(&xin[i], &complexGeometryX[i], sizeof(fftw_complex));
        /*if (i == 1)
        {
            std::cout << "real: " << xin[i][0] << " imag: " << xin[i][1] << std::endl;
        }*/
    }

    fftw_plan xp = fftw_plan_dft_2d(n, n,
                xin,
                xout,
                FFTW_BACKWARD,
                FFTW_ESTIMATE);

    fftw_execute(xp);
    //std::cout << "input real: " << xin[1][0]<< " imag: " << xout[1][1] << std::endl;
    //std::cout << "output real: " << xout[1][0] / 65536.0 << " imag: " << xout[1][1] / 65536.0 << std::endl;

    fftw_complex *yin;
    fftw_complex *yout;

    yin = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * complexGeometryY.size());
    yout = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * complexGeometryY.size());

    for (int i = 0 ;  i < complexGeometryY.size() ; i++)
    {
        memcpy(&yin[i], &complexGeometryY[i], sizeof(fftw_complex));
    }

    fftw_plan yp = fftw_plan_dft_2d(256, 256,
                yin,
                yout,
                FFTW_BACKWARD,
                FFTW_ESTIMATE);

    fftw_execute(yp);
    
    updateMesh(zout, xout, yout);

    fftw_destroy_plan(zp);
    fftw_destroy_plan(xp);
    fftw_destroy_plan(yp);

    fftw_free(zin);
    fftw_free(zout);
    fftw_free(xin);
    fftw_free(xout);
    fftw_free(yin);
    fftw_free(yout);
}

// Helper function for waveIDFT, transfers the results from IFFT to the z component of the wave model mesh
void waveModelCPU::updateMesh(fftw_complex* zout, fftw_complex* xout, fftw_complex* yout)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            geometry[3 * (i * n + j)] = (j * 1000.0f / 256.0f) + xout[i * n + j][0] / 65536.0;
            geometry[3 * (i * n + j) + 1] = (i * 1000.0f / 256.0f) + yout[i * n + j][0] / 65536.0;
            GLfloat resultZ = std::sqrt(std::pow(zout[i * n + j][0] / 65536.0, 2.0) + std::pow(zout[i * n + j][1] / 65536.0, 2.0));
            geometry[3 * (i * n + j) + 2] = resultZ;
        }

    }
}

// Calcualtes the normal vectors for each vertex
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

// Helper function for waveNorm, calculates the dy variables in waveNorm function
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

// Helper function for waveNorm, calculates the dx variables in waveNorm function
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

// Initializes the x and y components of the wave model mesh
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

// Initializes index array for EBO
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

// Updates complex wave model grid
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

            std::complex<double> complexAmp1 = complexGeometryZ[(x + maxLimit) * n + y + maxLimit];

            int negx = int((-1.0 * kx * LX) / (2 * M_PI));
            int negy = int((-1.0 * ky * LY) / (2 * M_PI));

            std::complex<double> complexAmp2 = complexGeometryZ[(negx + maxLimit) * n + negy + maxLimit];

            std::complex<double> result = complexAmp1 * std::exp(imaginary * omega * dt);
            complexGeometryZ[(x + maxLimit) * n + y + maxLimit] = result;

            double k = std::sqrt(std::pow(kx, 2.0) + std::pow(ky, 2.0));

            //if (i == -128)
            //    std::cout << "x: " << (imaginary * kx * spectrum) / k << std::endl;
            if (k <= 0.000001)
            {
                complexGeometryX[(x + maxLimit) * n + y + maxLimit] = std::complex<double>(0.0, 0.0);
                complexGeometryY[(x + maxLimit) * n + y + maxLimit] = std::complex<double>(0.0, 0.0);
            }
            else
            {
                complexGeometryX[(x + maxLimit) * n + y + maxLimit] = (imaginary * kx * result) / k;
                complexGeometryY[(x + maxLimit) * n + y + maxLimit] = (imaginary * ky * result) / k;
            }
        }
    }
}

// Updates the VBO with new vertex attribute data
void waveModelCPU::updateBuffer(GLuint ID)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, geometry.size() * sizeof(GLfloat), &geometry[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
