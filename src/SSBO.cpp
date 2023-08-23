#include"../include/SSBO.h"

SSBO::SSBO()
{
    std::cout << "No size or index specified" << std::endl;
}

// Constructor that generates a Vertex Buffer Object and links it to vertices
SSBO::SSBO(int size, int index)
{
    bufferSize = size;
    bufferIndex = index;

	glGenBuffers(1, &ID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndex, ID);
}

// Binds the VBO
void SSBO::Bind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
}

void SSBO::BindBase()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndex, ID);
}

void SSBO::Print(int low, int high)
{
    void* ssboData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    
    for (size_t i = 0; i < bufferSize; i++)
    {
        if (i >= low && i < high)
        {
            glm::vec2 temp;
            memcpy(&temp, &ssboData, sizeof(glm::vec2));
            std::cout << "Data at i: " << i << " real: " << temp.x << " imag: " << temp.y << std::endl;
        }
    }
}

// Unbinds the VBO
void SSBO::Unbind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

// Deletes the VBO
void SSBO::Delete()
{
	glDeleteBuffers(1, &ID);
}


SSBO::~SSBO()
{
	Delete();
}