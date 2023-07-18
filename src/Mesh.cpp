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
        //ExtractMeshData(data);
        std::string encodedData = data["buffers"][0]["uri"];
        DecodeMeshData(encodedData);

        int i = 0;
        for (auto& [key, val] : data.at("nodes").items())
        {
            int mesh = val.at("mesh");
            int accessor = data.at("meshes").at(mesh).at("primitives").at(0).at("indices");
            int bufferView = data.at("accessors").at(accessor).at("bufferView");

            int bufferViewOffset = data.at("bufferViews").at(bufferView).at("byteOffset");
            int size = data.at("accessors").at(accessor).at("count");
            int repititions = data.at("nodes").size();
            /*int offset = data.at("accessors").at(accessor).at("byteOffset");
            offset += bufferViewOffset;*/
            int length = bufferViewOffset + size * 2;
            int byteSize = 2;
            std::cout << "offset: " << bufferViewOffset << " length: " << length << " byteSize: " << byteSize << std::endl;
            ExtractIndices(decodedMeshData, length, bufferViewOffset, byteSize, i * size);
            i++;
        }
        
        for (auto& [key, val] : data.at("nodes").items())
        {
            int mesh = val.at("mesh");

            glm::vec3 translation(0.f, 0.f, 0.f);
            if (val.find("translation") != val.end())
            {
                translation[0] = val.at("translation").at(0);
                translation[1] = val.at("translation").at(1);
                translation[2] = val.at("translation").at(2);
            }

            int accessor = data.at("meshes").at(mesh).at("primitives").at(0).at("attributes").at("POSITION");
            int bufferView = data.at("accessors").at(accessor).at("bufferView");
            int bufferViewOffset = data.at("bufferViews").at(bufferView).at("byteOffset");
            int size = data.at("accessors").at(accessor).at("count");
            int length = bufferViewOffset + size * 12;
            int byteSize = sizeof(float);
            std::cout << "offset: " << bufferViewOffset << " length: " << length << " byteSize: " << byteSize << std::endl;
            ExtractVertices(decodedMeshData, length, bufferViewOffset, byteSize, translation);
        }

        normalsOffset = geometry.size();

        for (auto& [key, val] : data.at("nodes").items())
        {
            int mesh = val.at("mesh");

            glm::vec3 translation(0.f, 0.f, 0.f);

            int accessor = data.at("meshes").at(mesh).at("primitives").at(0).at("attributes").at("NORMAL");
            int bufferView = data.at("accessors").at(accessor).at("bufferView");
            int bufferViewOffset = data.at("bufferViews").at(bufferView).at("byteOffset");
            
            int size = data.at("accessors").at(accessor).at("count");
            int length = bufferViewOffset + size * 12;
            int byteSize = sizeof(float);
            std::cout << "offset: " << bufferViewOffset << " length: " << length << " byteSize: " << byteSize << std::endl;
            ExtractVertices(decodedMeshData, length, bufferViewOffset, byteSize, translation);
        }

        for (auto& [key, val] : data.at("nodes").items())
        {
            int mesh = val.at("mesh");

            glm::vec3 translation(0.f, 0.f, 0.f);

            int accessor = data.at("meshes").at(mesh).at("primitives").at(0).at("attributes").at("TEXCOORD_0");
            int bufferView = data.at("accessors").at(accessor).at("bufferView");
            int bufferViewOffset = data.at("bufferViews").at(bufferView).at("byteOffset");
            
            int size = data.at("accessors").at(accessor).at("count");
            int length = bufferViewOffset + size * 8;
            int byteSize = sizeof(float);
            std::cout << "offset: " << bufferViewOffset << " length: " << length << " byteSize: " << byteSize << std::endl;
            ExtractVertices(decodedMeshData, length, bufferViewOffset, byteSize, translation);
        }

        textureOffset = geometry.size();

        for (auto& [key, val] : data.at("nodes").items())
        {
            int mesh = val.at("mesh");

            glm::vec4 colour(0.f, 0.f, 0.f, 0.f);

            int material = data.at("meshes").at(mesh).at("primitives").at(0).at("material");
            colour[0] = data.at("materials").at(material).at("pbrMetallicRoughness").at("baseColorFactor").at(0);
            colour[1] = data.at("materials").at(material).at("pbrMetallicRoughness").at("baseColorFactor").at(1);
            colour[2] = data.at("materials").at(material).at("pbrMetallicRoughness").at("baseColorFactor").at(2);
            colour[3] = data.at("materials").at(material).at("pbrMetallicRoughness").at("baseColorFactor").at(3);
        }

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

void Mesh::DecodeMeshData(std::string encodedData)
{
    encodedData.erase(0, 37);
    decodedMeshData = base64_decode(encodedData);
}

void Mesh::ExtractIndices(std::string& data, int length, int offset, int size, int iteration)
{
    int temp = 0;
    for (int i = offset; i < length; i += size)
    {
        for (int j = i; j < (i + size); j++)
        {
            temp = temp + data[j] * std::pow(2, 8*(i + size - j - 1));
        }

        indices.push_back(temp / 256 + iteration);

        temp = 0;
    }

    /*for (GLuint index : indices)
    {
        std::cout << "index: " << index << std::endl;
    }*/
}

void Mesh::ExtractVertices(std::string& data, int length, int offset, int size, glm::vec3 translation)
{
    int temp = 0;
    for (int i = offset; i < length; i += size)
    {
        for (int j = i; j < (i + size); j++)
        {
            temp = temp + abs(float(data[j])) * std::pow(2, 8*(i + size - j - 1));
        }

        geometry.push_back(temp / float(32831) + translation[i % 3]);

        temp = 0;
    }
    /*std::cout << geometry.size() << std::endl;
    for (GLfloat vertex : geometry)
    {
        std::cout << "vertex: " << vertex << std::endl;
    }*/
}
