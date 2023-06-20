#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "glad/glad.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include "base64.h"

using json = nlohmann::json;

class Mesh
{
    public:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

        Mesh(const char* filename);
        ~Mesh();

    private:
        void ExtractIndices(std::string& data, int length, int offset, int size);
        void ExtractVertices(std::string& data, int length, int offset, int size);

};

#endif