#include "../include/Mesh.h"

Mesh::Mesh(const char* filename)
{
    int indexOffset = 0;
    int vertexOffset = 0;
    int indexLength = 0;
    int vertexLength = 0;
    std::string transmittedData = "";
    std::ifstream file(filename);

    json data = json::parse(file);

    for (auto& el : data.items()) {
        if (el.key() == "buffers")
        {
            for (auto& el1 : el.value()[0].items())
            {
                if (el1.key() == "uri")
                {
                    transmittedData = el1.value();
                }
            }
        }
        else if (el.key() == "bufferViews")
        {
            for (auto& el1 : el.value()[0].items())
            {
                if (el1.key() == "byteOffset")
                {
                    indexOffset = el1.value();
                }
                else if (el1.key() == "byteLength")
                {
                    indexLength = el1.value();
                }
            }
            for (auto& el1 : el.value()[1].items())
            {
                if (el1.key() == "byteOffset")
                {
                    vertexOffset = el1.value();
                }
                else if (el1.key() == "byteLength")
                {
                    vertexLength = el1.value();
                }
            }
        }
    }

    transmittedData.erase(0, 37);
    transmittedData = base64_decode(transmittedData);

    ExtractIndices(transmittedData, indexLength + indexOffset, indexOffset, 2);
    ExtractVertices(transmittedData, vertexLength + vertexOffset, vertexOffset, sizeof(float));
}

Mesh::~Mesh()
{
    std::cout << "Called Mesh deconstructor" << std::endl;
}

void Mesh::ExtractIndices(std::string& data, int length, int offset, int size)
{
    int temp = 0;
    for (int i = offset; i < length; i += size)
    {
        for (int j = i; j < (i + size); j++)
        {
            temp = temp + data[j] * std::pow(2, 8*(i + size - j - 1));
        }

        indices.push_back(temp / 256);

        temp = 0;
    }
}

void Mesh::ExtractVertices(std::string& data, int length, int offset, int size)
{
    int temp = 0;
    int check = 1;
    for (int i = offset; i < length; i += size)
    {
        for (int j = i; j < (i + size); j++)
        {
            temp = temp + abs(float(data[j])) * std::pow(2, 8*(i + size - j - 1));
        }

        vertices.push_back(temp / float(32831));

        if (check % 3 == 0)
        {
            vertices.push_back(0.92f);
            vertices.push_back(0.86f);
            vertices.push_back(0.76f);
        }

        check++;

        temp = 0;
    }

    for (GLfloat vertex : vertices)
    {
        std::cout << "vertex: " << vertex << std::endl;
    }
}