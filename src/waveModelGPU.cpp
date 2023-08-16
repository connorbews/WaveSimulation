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
	glBufferData(GL_SHADER_STORAGE_BUFFER, 64 * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	initialize.Activate();

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	GLuint ssbo1;
	glGenBuffers(1, &ssbo1);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo1);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 64 * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo1);

	horizontalidft.Activate();

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbo1);

	GLuint ssbo2;
	glGenBuffers(1, &ssbo2);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo2);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 64 * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW); //sizeof(data) only works for statically sized C/C++ arrays.
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo2);

	verticalidft.Activate();

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, ssbo2);
}