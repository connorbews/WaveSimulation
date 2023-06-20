#include "../include/Mesh.h"

Mesh::Mesh(const nlohmann::json& data)
{
    //LoadMeshData(data);
}

Mesh::~Mesh()
{
    std::cout << "Called Mesh deconstructor" << std::endl;
}

void Mesh::LoadMeshData(const nlohmann::json& data)
{
    ExtractMeshData(data);
    ParseIndexOffset(data);
    ParseIndexLength(data);
    ParseVertexOffset(data);
    ParseVertexLength(data);
    ExtractIndices(decodedMeshData, indexLength + indexOffset, indexOffset, 2);
    ExtractVertices(decodedMeshData, vertexLength + vertexOffset, vertexOffset, sizeof(float));
}

void Mesh::ExtractMeshData(const nlohmann::json& data)
{
    std::string encodedData = data["buffers"][0]["uri"];
    DecodeMeshData(encodedData);
}

void Mesh::DecodeMeshData(std::string encodedData)
{
    encodedData.erase(0, 37);
    decodedMeshData = base64_decode(encodedData);
}

void Mesh::ParseIndexOffset(const nlohmann::json& data)
{
    indexOffset = data["bufferViews"][0]["byteOffset"];
}

void Mesh::ParseIndexLength(const nlohmann::json& data)
{
    indexLength = data["bufferViews"][0]["byteLength"];
}

void Mesh::ParseVertexOffset(const nlohmann::json& data)
{
    vertexOffset = data["bufferViews"][1]["byteOffset"];
}

void Mesh::ParseVertexLength(const nlohmann::json& data)
{
    vertexLength = data["bufferViews"][1]["byteLength"];
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