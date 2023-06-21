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
        ~Mesh();

    private:
		void LoadMeshData(const nlohmann::json& data);
		void ExtractMeshData(const nlohmann::json& data) noexcept(false);
		void DecodeMeshData(std::string encodedData);
		void ParseIndexOffset(const nlohmann::json& data) noexcept(false);
		void ParseVertexOffset(const nlohmann::json& data) noexcept(false);
		void ParseIndexLength(const nlohmann::json& data) noexcept(false);
		void ParseVertexLength(const nlohmann::json& data) noexcept(false);

        void ExtractIndices(std::string& data, int length, int offset, int size) noexcept(false);
        void ExtractVertices(std::string& data, int length, int offset, int size) noexcept(false);

};

#endif