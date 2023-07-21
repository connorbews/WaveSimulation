#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "glad/glad.h"
#include "nlohmann/json.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include "base64.h"


class Mesh
{
    public:
        std::vector<GLuint> indices;
		std::vector<GLfloat> geometry;
		std::string decodedMeshData;

		std::vector<GLfloat> translation;

		int indexOffset;
		int indexLength;
    	int primitiveOffset;
    	int primitiveLength;

		int normalsOffset;
		int textureOffset;

		std::vector<float> colour;

        Mesh(const nlohmann::json& data);
        ~Mesh();

    private:
		void LoadMeshData(const nlohmann::json& data);
		void ExtractMeshData(const nlohmann::json& data) noexcept(false);
		void DecodeMeshData(std::string encodedData);

		int ParseOffsetData(const nlohmann::json& data, int index) noexcept(false);
		int ParseLengthData(const nlohmann::json& data, int index) noexcept(false);

        void ExtractIndices(std::string& data, int length, int offset, int size, int iteration) noexcept(false);
        void ExtractVertices(std::string& data, int length, int offset, int size, glm::vec3 translation) noexcept(false);

};

#endif