#include "../include/Mesh.h"

Mesh::Mesh(const char* filename)
{
    std::string transmittedData = "";
    std::ifstream file(filename);

    json data = json::parse(file);

    for (auto& el : data.items()) {
        if (el.key() == "buffers")
        {
            //std::cout << el.key() << " : " << "Value: " << el.value() << "\n";
            for (auto& el1 : el.value()[0].items())
            {
                if (el1.key() == "uri")
                {
                    transmittedData = el1.value();
                }
            }
        }
    }

    std::cout << "successfully read file: " << transmittedData << std::endl;

    transmittedData.erase(0, 37);

    std::cout << "successfully edited string: " << transmittedData << std::endl;

    std::string decodedData = base64_decode(transmittedData);

    std::cout << "Decoded string: " << decodedData << std::endl;
}

Mesh::~Mesh()
{
    std::cout << "Called Mesh deconstructor" << std::endl;
}