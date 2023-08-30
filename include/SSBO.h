#ifndef SSBO_CLASS_H
#define SSBO_CLASS_H

#include <iostream>
#include <cstring>
#include <vector>
#include<glad/glad.h>
#include "glm/glm.hpp"

class SSBO
{
public:
	GLuint ID;

    int bufferSize;

    int bufferIndex;

	SSBO();
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	SSBO(int size, int index);

	~SSBO();

	// Binds the VBO
	void Bind();
	void BindBase();

    void Print(int low, int high);
	// Unbinds the VBO
	void Unbind();
	// Deletes the VBO
	void Delete();
};

#endif