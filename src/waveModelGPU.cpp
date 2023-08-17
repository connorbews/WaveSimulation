#include "../include/waveModelGPU.h"

waveModelGPU::waveModelGPU()
{
    waveModelGPU(256);
}

waveModelGPU::waveModelGPU(int size)
{
    n = size;

    ComputeShader initialize("resources/shaders/initialize.comp");
	ComputeShader horizontalidft("resources/shaders/horizontalidft.comp");
	ComputeShader verticalidft("resources/shaders/verticalidft.comp");
	ComputeShader wavePropagation("resources/shaders/wavePropagation.comp");

    GLuint ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, std::pow(size, 2) * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	initialize.Activate();

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	GLuint ssbo1;
	glGenBuffers(1, &ssbo1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo1);
	glBufferData(GL_SHADER_STORAGE_BUFFER, std::pow(size, 2) * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo1);

	horizontalidft.Activate();

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo1);

	GLuint ssbo2;
	glGenBuffers(1, &ssbo2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 3 * std::pow(size, 2) * sizeof(float), NULL, GL_DYNAMIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo2);

	verticalidft.Activate();

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo2);

	void* ssboData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    // Now you can access the data just like any other array
    float* ssboArray = static_cast<float*>(ssboData);
    for (size_t i = 0; i < 3 * std::pow(size, 2); i++) {
        geometry.push_back(ssboArray[i] );
    }

	for (int i = 0; i < size - 1; i++)
    {
        for (int j = 0; j < size - 1; j++)
        {
            index.push_back(i * size + j);
            index.push_back((i + 1) * size + j);
            index.push_back(i * size + j + 1);

            index.push_back((i + 1) * size + j + 1);
            index.push_back(i * size + j + 1);
            index.push_back((i + 1) * size + j);
        }
    }
}