#include "../include/waveModelGPU.h"

waveModelGPU::waveModelGPU()
{
    waveModelGPU(256);
}

waveModelGPU::waveModelGPU(int size) : 
	initialize("resources/shaders/initialize.comp"),
	horizontalidft("resources/shaders/horizontalidft.comp"),
	verticalidft("resources/shaders/verticalidft.comp"),
	wavePropagation("resources/shaders/wavePropagation.comp"),
	normalCalculation("resources/shaders/normalvec.comp"),
	initializeBuffer(std::pow(size, 2), 0),
	horizontalOutBuffer(std::pow(size, 2), 1),
	verticalOutBuffer(6 * std::pow(size, 2), 2),
	geometry(6 * std::pow(size, 2), 0)
{
    n = size;

	waveInit();
	waveIDFT();
	waveNorm();

	waveMesh();
	waveIndex();
}

void waveModelGPU::updateModel(float dt)
{
	waveProp(dt);
	waveIDFT();
	waveNorm();

	//seg faults here:
	waveMesh();
}

void waveModelGPU::waveInit()
{
	initialize.Activate(8, 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void waveModelGPU::waveIDFT()
{
	horizontalidft.Activate(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	verticalidft.Activate(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void waveModelGPU::waveNorm()
{
	normalCalculation.Activate(8, 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void waveModelGPU::waveMesh()
{
	verticalOutBuffer.BindBase();

	// i think the issue is here
	void* ssboData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    // Now you can access the data just like any other array
    glm::vec2* ssboArray = static_cast<glm::vec2*>(ssboData);
    for (size_t i = 0; i < 6 * std::pow(n, 2); i++) {
        geometry[i] = std::sqrt(pow(ssboArray[i][0], 2) + pow(ssboArray[i][1], 2));
    }
}

void waveModelGPU::waveIndex()
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

void waveModelGPU::waveProp(float dt)
{
	glUniform1f(glGetUniformLocation(wavePropagation.ID, "dt"), dt);

	wavePropagation.Activate(8, 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
