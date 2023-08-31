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
	verticalOutBuffer(6 * std::pow(size, 2), 2),
	geometry(6 * std::pow(size, 2), 0)
{
    n = size;
	normalsOffset = 3 * std::pow(n, 2);
	
	waveIndex();
	waveInit();
	waveIDFT();
	waveNorm();
	
	geometryMapInit();
}

void waveModelGPU::updateModel(GLuint ID)
{
	waveDt();
	waveProp();
	waveIDFT();
	waveNorm();
	
	//verticalOutBuffer.Print(0, 256);

	geometryMap(ID);

	//std::cout << "geometry[0]: " << geometry[90] << std::endl;
	//std::cout << "geometry[1]: " << geometry[91] << std::endl;
	//std::cout << "geometry[2]: " << geometry[92] << std::endl;
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

void waveModelGPU::geometryMapInit()
{
	verticalOutBuffer.BindBase();
	void* ssboData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	int size = 6 * std::pow(n, 2);
	
	for (int i = 0; i < size; i++)
    {
		glm::vec2 temp;
		memcpy(&temp, ssboData, sizeof(glm::vec2));
		/*if (i >= 90 && i < 93)
		{
			std::cout << "temp.x: " << temp.x << std::endl;
		}*/
		GLfloat result = std::sqrt(std::pow(temp.x, 2.0f) + std::pow(temp.y, 2.0f));
		geometry[i] = result;
		ssboData = static_cast<char*>(ssboData) + sizeof(glm::vec2);
    }

    GLboolean val = glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    if (val == GL_FALSE)
    {
        std::cerr << "contents were corrupted" << std::endl;
    }
}

void waveModelGPU::geometryMap(GLuint ID)
{
	verticalOutBuffer.BindBase();
	void* ssboData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	int size = 6 * std::pow(n, 2);
	
	for (int i = 0; i < size; i++)
    {
		glm::vec2 temp;
		memcpy(&temp, ssboData, sizeof(glm::vec2));
		/*if (i >= 90 && i < 93)
		{
			std::cout << "temp.x: " << temp.x << std::endl;
		}*/
		GLfloat result = std::sqrt(std::pow(temp.x, 2.0f) + std::pow(temp.y, 2.0f));
		geometry[i] = result;
		ssboData = static_cast<char*>(ssboData) + sizeof(glm::vec2);
    }

    GLboolean val = glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    if (val == GL_FALSE)
    {
        std::cerr << "contents were corrupted" << std::endl;
    }

	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, geometry.size() * sizeof(GLfloat), &geometry[0]);
	//std::cout << "size: " << geometry.size() * sizeof(GLfloat) << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
