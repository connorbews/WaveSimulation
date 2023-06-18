#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "glad/glad.h"
#include <vector>

class Mesh
{
    public:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        Mesh();
        Mesh(const char* filename);
        ~Mesh();
};

#endif