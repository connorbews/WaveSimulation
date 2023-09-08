#include<filesystem>
namespace fs = std::filesystem;

#include<math.h>

#include "include/WaveSimProj/Setup/OpenGLSetup.h"
#include "include/WaveSimProj/Setup/GLFWSetup.h"

#include "include/WaveSimProj/ObjectShaderClass.h"
#include "include/WaveSimProj/ComputeShaderClass.h"
#include "include/WaveSimProj/waveModelGPU.h"
#include "include/WaveSimProj/VAO.h"
#include "include/WaveSimProj/VBO.h"
#include "include/WaveSimProj/EBO.h"
#include "include/stb/stb_image.h"
#include "include/WaveSimProj/Texture.h"
#include "include/WaveSimProj/Camera.h"
#include "include/WaveSimProj/Model.h"
#include "include/WaveSimProj/waveModelCPU.h"


GLfloat lightVertices[] = 
{
	-10.0f, -10.0f,  10.0f,
	-10.0f, -10.0f, -10.0f,
	 10.0f, -10.0f, -10.0f,
	 10.0f, -10.0f,  10.0f,
	-10.0f,  10.0f,  10.0f,
	-10.0f,  10.0f, -10.0f,
	 10.0f,  10.0f, -10.0f,
	 10.0f,  10.0f,  10.0f
};

GLuint lightIndices[] = 
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main()
{
	
	Camera camera(800, 800, glm::vec3(500.0f, 500.0f, 700.0f));

	GLFWSetup glfwScreen(&camera);

	OpenGLSetup openGLSetup;
	
	int n = 256;

	//waveModelGPU waveGPU(n);
	waveModelCPU waveModelCPU(n);
	
	// Generates Shader object using shaders defualt.vert and default.frag
	ObjectShader shaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(&waveModelCPU.geometry[0], waveModelCPU.geometry.size() * sizeof(GLfloat));
	//VBO VBO1(&waveGPU.geometry[0], waveGPU.geometry.size() * sizeof(GLfloat));

	// Generates Element Buffer Object and links it to indices
	EBO EBO1(&waveModelCPU.index[0], waveModelCPU.index.size() * sizeof(GLuint));
	//EBO EBO1(&waveGPU.index[0], waveGPU.index.size() * sizeof(GLuint));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 0, (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 0, (void*)(waveModelCPU.normalsOffset * sizeof(float)));
	//VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 0, (void*)0);
	//VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 0, (void*)(waveGPU.normalsOffset * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	ObjectShader lightShader("resources/shaders/light.vert", "resources/shaders/light.frag");
	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 0, (void*)0);

	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 lightPos = glm::vec3(500.0f, 500.0f, 100.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 wavePos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 wavemodel = glm::mat4(1.0f);
	wavemodel = glm::translate(wavemodel, wavePos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(wavemodel));
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPosition"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "matColour"), 0.005960569716989994f, 0.0f, 0.8000000715255737, 1.0f);

	while (glfwScreen.ShouldWindowClose())
	{
		openGLSetup.ProcessErrors();
		openGLSetup.SetBackgroundColour(0.07f, 0.13f, 0.17f, 1.0f);
		openGLSetup.CleanBuffer();

		shaderProgram.Activate();
		camera.updateMatrix(45.0f, 0.1f, 10000.0f);
		camera.Matrix(shaderProgram, "camMatrix");

		VAO1.Bind();
		//glDrawElements(GL_TRIANGLES, waveGPU.index.size() * sizeof(GLuint) / sizeof(int), GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, waveModelCPU.index.size() * sizeof(GLuint) / sizeof(int), GL_UNSIGNED_INT, 0);
		
		//waveGPU.updateModel(VBO1.ID);
		waveModelCPU.updateModel(VBO1.ID);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");

		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwScreen.UpdateScreenInputs();
	}

	shaderProgram.Delete();
	return 0;
}
