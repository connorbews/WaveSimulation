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
	initializeBuffer.Print(0, 32);
	waveIDFT();
	waveNorm();

	waveMesh();
	waveIndex();
}

void waveModelGPU::updateModel(float dt)
{
	waveProp(dt);
	//initializeBuffer.Print(0, 32);
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
	//verticalOutBuffer.Print(65436, 65536);

	void* ssboData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    for (int i = 0; i < 6 * std::pow(n, 2); i++)
	{
		glm::vec2 temp;
		memcpy(&temp, &ssboData, sizeof(glm::vec2));
        geometry[i] = std::sqrt(std::pow(temp.x, 2) + std::pow(temp.y, 2));
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
	//std::cout << glGetUniformLocation(wavePropagation.ID, "dt") << std::endl;
	glUniform1f(glGetUniformLocation(wavePropagation.ID, "dt"), 1.0f);

	wavePropagation.Activate(8, 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
