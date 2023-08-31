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
	dt("resources/shaders/dt.comp"),
	normalCalculation("resources/shaders/normalvec.comp"),
	initializeBuffer(std::pow(size, 2) + 1, 0),
	horizontalOutBuffer(std::pow(size, 2), 1),
	verticalOutBuffer(6 * std::pow(size, 2), 2)
{
    n = size;

	waveIndex();
	waveInit();
	waveIDFT();
	//waveNorm();
}

void waveModelGPU::updateModel()
{
	waveDt();
	waveProp();
	waveIDFT();
	//waveNorm();
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

void waveModelGPU::waveProp()
{
	wavePropagation.Activate(8, 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void waveModelGPU::waveDt()
{
	dt.Activate(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}
