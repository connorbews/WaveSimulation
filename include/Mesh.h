#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include "glad/glad.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include "base64.h"


class Mesh
{
    public:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;
		std::string decodedMeshData;
		int indexOffset;
    	int vertexOffset;
    	int indexLength;
    	int vertexLength;

        Mesh(const nlohmann::json& data);
		void LoadMeshData(const nlohmann::json& data);
        ~Mesh();

    private:
		void ExtractMeshData(const nlohmann::json& data);
		void DecodeMeshData(std::string encodedData);
		void ParseIndexOffset(const nlohmann::json& data);
		void ParseVertexOffset(const nlohmann::json& data);
		void ParseIndexLength(const nlohmann::json& data);
		void ParseVertexLength(const nlohmann::json& data);

        void ExtractIndices(std::string& data, int length, int offset, int size);
        void ExtractVertices(std::string& data, int length, int offset, int size);

};

#endif