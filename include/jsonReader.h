#ifndef JSONREADER_CLASS_H
#define JSONREADER_CLASS_H

#include "glad/glad.h"
#include "nlohmann/json.hpp"

class jsonReader
{
    public:
        getMeshFromFile(const char* filename, Mesh& mesh);

};

#endif