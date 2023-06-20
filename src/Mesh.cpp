#include "../include/Mesh.h"

Mesh::Mesh(const char* filename)
{
    std::string transmittedData = "";
    std::ifstream file(filename);

    json data = json::parse(file);

    for (auto& el : data.items()) {
        if (el.key() == "buffers")
        {
            //std::cout << el.key() << " : " << "Value: " << el.value() << "\n";
            for (auto& el1 : el.value()[0].items())
            {
                if (el1.key() == "uri")
                {
                    transmittedData = el1.value();
                }
            }
        }
    }

    std::cout << "successfully read file: " << transmittedData << std::endl;

    transmittedData.erase(0, 37);

    std::cout << "successfully edited string: " << transmittedData << std::endl;

    transmittedData = base64_decode(transmittedData);

    ExtractIndices(transmittedData, 6, 0, sizeof(int));
    ExtractVertices(transmittedData, transmittedData.size(), 8, sizeof(float));
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

        std::cout << (temp / 256) << std::endl;
        indices.push_back(temp / 256);

        temp = 0;
    }
}

void Mesh::ExtractVertices(std::string& data, int length, int offset, int size)
{
    int temp = 0;
    for (int i = offset; i < length; i += size)
    {
        for (int j = i; j < (i + size); j++)
        {
            temp = temp + abs(float(data[j])) * std::pow(2, 8*(i + size - j - 1));
        }

        std::cout << (temp / float(32831)) << std::endl;
        vertices.push_back(temp / float(32831));

        temp = 0;
    }
}