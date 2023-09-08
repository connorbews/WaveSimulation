#include "../../include/WaveSimProj/ObjectShaderClass.h"

// Constructor that build the Shader Program from 2 different shaders
ObjectShader::ObjectShader(const char* vertexFile, const char* fragmentFile)
{
    // Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	
	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	
	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}