#include "../../include/WaveSimProj/Model.h"

Model::Model(const char* filename) : Mesh::Mesh(Loader::fileReader(filename))
{ 
    
}

Model::~Model()
{
    
}
