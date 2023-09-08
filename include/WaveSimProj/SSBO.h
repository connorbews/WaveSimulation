#ifndef SSBO_CLASS_H
#define SSBO_CLASS_H

#include <iostream>
#include <cstring>
#include <vector>
#include "../glad/glad.h"
#include <glm/glm.hpp>

class SSBO
{
public:
	GLuint ID;
    int bufferSize;
    int bufferIndex;

	// Constructor called with no size or index specified, calling this is an error
	SSBO();
	// Constructor that generates a Shader Storage Buffer Object and binds it to the binding point at index "index"
	SSBO(int size, int index);

	// Prints the contents of the SSBO from firstIndex to lastIndex 
    void debugPrint(int firstIndex = 0, int lastIndex = 0);
	// Binds the SSBO
	void Bind();
	// Binds the SSBO to a binding point at "bufferIndex"
	void BindBase();
	// Unbinds the SSBO
	void Unbind();
	// Deletes the SSBO
	void Delete();

	// Deconstructor for SSBO
	~SSBO();
};

#endif