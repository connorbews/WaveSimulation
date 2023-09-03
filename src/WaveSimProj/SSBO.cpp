#include"../../include/WaveSimProj/SSBO.h"

// Constructor called with no size or index specified, calling this is an error
SSBO::SSBO()
{
    std::cerr << "No size or index specified" << std::endl;
}

// Constructor that generates a Shader Storage Buffer Object and binds it to the binding point at index "index"
SSBO::SSBO(int size, int index)
{
    bufferSize = size;
    bufferIndex = index;
    
	glGenBuffers(1, &ID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize * sizeof(glm::vec2), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndex, ID);
}

// Prints the contents of the SSBO from firstIndex to lastIndex
void SSBO::debugPrint(int firstIndex, int lastIndex)
{
    if (firstIndex == lastIndex)
    {
        std::cerr << "no values are printed because the arguments for this function are the same" << std::endl;
    }

    BindBase();
    void* ssboData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    
    for (size_t i = 0; i < bufferSize; i++)
    {
        glm::vec2 temp;
        if (i >= firstIndex && i < lastIndex)
        {
            memcpy(&temp, ssboData, sizeof(glm::vec2));
            std::cout << "Data at i: " << i << " real: " << temp.x << " imag: " << temp.y << std::endl;
            ssboData = static_cast<char*>(ssboData) + sizeof(glm::vec2);
        }
    }

    GLboolean val = glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    if (val == GL_FALSE)
    {
        std::cerr << "contents were corrupted" << std::endl;
    }
}

// Binds the SSBO
void SSBO::Bind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);
}

// Binds the SSBO to a binding point at "bufferIndex"
void SSBO::BindBase()
{
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferIndex, ID);
}

// Unbinds the SSBO
void SSBO::Unbind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

// Deletes the SSBO
void SSBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

// Deconstructor for SSBO
SSBO::~SSBO()
{
	Delete();
}