#include "../../include/WaveSimProj/Loader.h"

nlohmann::json Loader::fileReader(const char* filename)
{
    nlohmann::json fileData;

    std::ifstream file(filename);

    if (file)
    {
        try
        {
            fileData = nlohmann::json::parse(file);
        }
        catch (nlohmann::json::parse_error& ex)
        {
            std::cerr << "Parsing error: " << ex.byte << std::endl;
        }
        
        file.close();
        return fileData;
    }
    else
    {
        std::cout << "There was an error opening the file, check the file path" << std::endl;
        return NULL;
    }
}