#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "Mesh.h"
#include "Loader.h"

class Model : public Mesh
{
    public:
        Model(const char* filename);
        ~Model();
};

#endif