#include "../include/Mesh.h"

Mesh::Mesh(const nlohmann::json& data)
{
    LoadMeshData(data);
}

Mesh::~Mesh()
{
    
}

void Mesh::LoadMeshData(const nlohmann::json& data)
{
    try
    {
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
            
            int length = bufferViewOffset + size * 2;
            int byteSize = 2;
            
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
            
            ExtractVertices(decodedMeshData, length, bufferViewOffset, byteSize, translation);
        }

        textureOffset = geometry.size();

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

            ExtractVertices(decodedMeshData, length, bufferViewOffset, byteSize, translation);
        }

        for (auto& [key, val] : data.at("nodes").items())
        {
            int mesh = val.at("mesh");

            int material = data.at("meshes").at(mesh).at("primitives").at(0).at("material");

            colour.push_back(data.at("materials").at(material).at("pbrMetallicRoughness").at("baseColorFactor").at(0));
            colour.push_back(data.at("materials").at(material).at("pbrMetallicRoughness").at("baseColorFactor").at(1));
            colour.push_back(data.at("materials").at(material).at("pbrMetallicRoughness").at("baseColorFactor").at(2));
            colour.push_back(data.at("materials").at(material).at("pbrMetallicRoughness").at("baseColorFactor").at(3));
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
            int data1 = 0;
            if ((int)data[j] < 0)
            {
                data1 = (int)data[j] + 256;
            }
            else
            {
                data1 = (int)data[j];
            }
            temp = temp + abs(data1 * std::pow(2, 8*(j - i + 1)));
            
        }
        
        indices.push_back(temp / 256 + iteration);

        temp = 0;
    }
}

void Mesh::ExtractVertices(std::string& data, int length, int offset, int size, glm::vec3 translation)
{
    int temp = 0;
    for (int i = offset; i < length; i += size)
    {
        unsigned char data1 = data[i];
        unsigned char data2 = data[i + 1];
        unsigned char data3 = data[i + 2];
        unsigned char data4 = data[i + 3];

        // From Victor Gordan Line 172: https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%2013%20-%20Model%20Loading/Model.cpp
        // ------
        unsigned char bytes[] = { data1, data2, data3, data4 };
		float value;
		std::memcpy(&value, bytes, sizeof(float));
        // ------

        geometry.push_back(value + translation[i % 3]);

        temp = 0;
    }
}
