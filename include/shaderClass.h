#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <vector>

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		// Reference ID of the Shader Program
		GLuint ID;
		GLuint computeProgram;

		// Constructor that build the Shader Program from 2 different shaders
		Shader(const char* vertexFile, const char* fragmentFile);
		Shader(const char* computeFile);

		// Activates the Shader Program
		void Activate();
		void computeActivate();
		// Deletes the Shader Program
		void Delete();
		void computeDelete();

	private: 
		void compileErrors(unsigned int shader, const char* type);
};
#endif