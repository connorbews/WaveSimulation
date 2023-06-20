#include "../include/Model.h"

Model::Model(const char* filename) : Mesh::Mesh(fileData)
{
    std::ifstream file(filename);
    fileData = nlohmann::json::parse(file);

    file.close();

    LoadMeshData(fileData);
    std::cout << "successfully compiled program" << std::endl;
}

Model::~Model()
{
    std::cout << "successfully called model deconstructor" << std::endl;
}