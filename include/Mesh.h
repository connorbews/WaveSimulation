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
        std::vector<GLfloat> vertices/* {-0.5f, 0.0f, 0.5f,  0.83f, 0.70f, 0.44f,  0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f,  5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f, 0.83f, 0.70f, 0.44f,  0.0f, 0.0f,
	 0.5f, 0.0f, 0.5f,  0.83f, 0.70f, 0.44f,  5.0f, 0.0f,
	 0.0f, 0.8f, 0.0f,  0.92f, 0.86f, 0.76f,  2.5f, 5.0f}*/;
        std::vector<GLuint> indices/* = {0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4,}*/;

        Mesh(const char* filename);
        ~Mesh();

    private:
        void ExtractIndices(std::string& data, int length, int offset, int size);
        void ExtractVertices(std::string& data, int length, int offset, int size);

};

#endif