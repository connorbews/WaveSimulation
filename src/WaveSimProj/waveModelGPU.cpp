#include "../../include/WaveSimProj/waveModelGPU.h"

// Constructor called with no size specified, calling this is an error
waveModelGPU::waveModelGPU()
{
    std::cerr << "No size specified" << std::endl;
}

// Constructor that generates a waveModel of size: "size" x "size" to run on the GPU
waveModelGPU::waveModelGPU(int size) : 
	initCalc("resources/shaders/ComputeShaders/initCalc.comp"),
	horizontalIDFT("resources/shaders/ComputeShaders/horizontalIDFT.comp"),
	verticalIDFT("resources/shaders/ComputeShaders/verticalIDFT.comp"),
	wavePropagation("resources/shaders/ComputeShaders/wavePropagation.comp"),
	dtCalc("resources/shaders/ComputeShaders/dtCalc.comp"),
	normalCalc("resources/shaders/ComputeShaders/normalCalc.comp"),
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
	
	getPositionDataGPU();
}

// Updates each vertex of the waveModel and updates the VBO specified by "ID" 
void waveModelGPU::updateModel(GLuint ID)
{
	waveDt();
	waveProp();
	waveIDFT();
	waveNorm();

	getPositionDataGPU();
	updateBuffer(ID);
}

// Calls the initCalc shader
void waveModelGPU::waveInit()
{
	initCalc.Activate(8, 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

// Calls the horizontalIDFT and verticalIDFT shader's
void waveModelGPU::waveIDFT()
{
	horizontalIDFT.Activate(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	verticalIDFT.Activate(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

// Calls the normalCalc shader
void waveModelGPU::waveNorm()
{
	normalCalc.Activate(8, 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

// Initializes index array for EBO
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

// Calls the wavePropagation shader
void waveModelGPU::waveProp()
{
	wavePropagation.Activate(8, 8, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

// Calls the dtCalc shader
void waveModelGPU::waveDt()
{
	dtCalc.Activate(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

// Transfers the position and normal information from GPU to CPU to update the VBO
void waveModelGPU::getPositionDataGPU()
{
	verticalOutBuffer.BindBase();
	void* ssboData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	int size = 6 * std::pow(n, 2);
	
	for (int i = 0; i < size; i++)
    {
		glm::vec2 temp;
		memcpy(&temp, ssboData, sizeof(glm::vec2));
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

// Updates the VBO with new vertex attribute data
void waveModelGPU::updateBuffer(GLuint ID)
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferSubData(GL_ARRAY_BUFFER, 0, geometry.size() * sizeof(GLfloat), &geometry[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
