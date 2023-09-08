#ifndef LOADER_CLASS_H
#define LOADER_CLASS_H

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class Loader
{
    public:
        static nlohmann::json fileReader(const char* filename);
};

#endif