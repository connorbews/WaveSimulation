#include "../include/Mesh.h"

Mesh::Mesh(const nlohmann::json& data)
{   
    LoadMeshData(data);
}

Mesh::~Mesh()
{
    std::cout << "Called Mesh deconstructor" << std::endl;
}

void Mesh::LoadMeshData(const nlohmann::json& data)
{
    try
    {
        indexOffset = ParseOffsetData(data, 0);
        indexLength = ParseLengthData(data, 0);
        primitiveOffset = ParseOffsetData(data, 1);
        primitiveLength = ParseLengthData(data, 1);
        std::cout << "indexLength: " << indexLength << std::endl;
        std::cout << "indexOffset: " << indexOffset << std::endl;
        std::cout << "primitiveLength: " << primitiveLength << std::endl;
        std::cout << "primitiveLength: " << primitiveOffset << std::endl;
    }
    catch (const nlohmann::json::basic_json::out_of_range& ex)
    {
        std::cerr << "Whatever you were trying to access is not there: " << ex.what() << std::endl;
    }
    catch (const nlohmann::json::basic_json::type_error& ex)
    {
        std::cerr << "Whatever you are passing to at is not a string or a number" << ex.what() << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Something else has gone wrong, sorry" << ex.what() << std::endl;
    }

    try
    {
        ExtractMeshData(data);
    }
    catch(const std::out_of_range& ex)
    {
        std::cerr << "Out of Range: " << ex.what() << std::endl;
    }
    catch(const std::invalid_argument& ex)
    {
        std::cerr << "Invalid Argument: " << ex.what() << std::endl;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Something else has gone wrong, sorry" << ex.what() << std::endl;
    }
    
}



void Mesh::ExtractMeshData(const nlohmann::json& data)
{
    std::string encodedData = data["buffers"][0]["uri"];
    DecodeMeshData(encodedData);
    nlohmann::json bufferViews = data.at("bufferViews");

    for (auto& it : bufferViews.items())
    {
        int target = it.value().at("target");
        if (target == GL_ELEMENT_ARRAY_BUFFER)
        {
            ExtractIndices(decodedMeshData, indexLength + indexOffset, indexOffset, 2);
        }
        else if (target == GL_ARRAY_BUFFER)
        {
            ExtractVertices(decodedMeshData, primitiveLength + primitiveOffset, primitiveOffset, sizeof(float));
        }
    }
}

void Mesh::DecodeMeshData(std::string encodedData)
{
    encodedData.erase(0, 37);
    decodedMeshData = base64_decode(encodedData);
}

int Mesh::ParseOffsetData(const nlohmann::json& data, int index)
{
    return data.at("bufferViews").at(index).at("byteOffset");
}

int Mesh::ParseLengthData(const nlohmann::json& data, int index)
{
    return data.at("bufferViews").at(index).at("byteLength");
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

    for (GLuint index : indices)
    {
        std::cout << "index: " << index << std::endl;
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

        vertices.push_back(temp / float(32831));

        temp = 0;
    }

    for (GLfloat vertex : vertices)
    {
        std::cout << "vertex: " << vertex << std::endl;
    }
}

/*int Mesh::ExtractNumNodes(const nlohmann::json& data)
{

}*/