#include "../../include/WaveSimProj/ComputeShaderClass.h"

ComputeShader::ComputeShader()
{
	std::cerr << "No source file has been specified" << std::endl;
}

ComputeShader::ComputeShader(const char* computeFile)
{
	std::string computeCode = get_file_contents(computeFile);

	const char* computeSource = computeCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(computeShader, 1, &computeSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(computeShader);
	compileErrors(computeShader, "COMPUTE");

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, computeShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	compileErrors(ID, "COMPUTE PROGRAM");

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(computeShader);
}


void ComputeShader::Activate(int workGroupx, int workGroupy, int workGroupz)
{
	glUseProgram(ID);
	glDispatchCompute(workGroupx, workGroupy, workGroupz);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}