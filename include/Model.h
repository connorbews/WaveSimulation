#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "Mesh.h"
#include "glad/glad.h"

class Model
{
    public:
        Mesh mesh;
        Model(const char* filename);
        ~Model();
};

#endif