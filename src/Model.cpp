#include "../include/Model.h"

Model::Model(const char* filename) : Mesh::Mesh(Loader::fileReader(filename))
{ 
    std::cout << "successfully compiled program" << std::endl;
}

Model::~Model()
{
    std::cout << "successfully called model deconstructor" << std::endl;
}
