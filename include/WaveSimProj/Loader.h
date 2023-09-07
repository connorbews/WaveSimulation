#ifndef LOADER_CLASS_H
#define LOADER_CLASS_H

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>

class Loader
{
    public:
        static nlohmann::json fileReader(const char* filename);
};

#endif