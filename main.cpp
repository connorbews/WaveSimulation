#include<filesystem>
namespace fs = std::filesystem;

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<math.h>

#include"include/ObjectShaderClass.h"
#include"include/ComputeShaderClass.h"
#include"include/waveModelGPU.h"
#include"include/VAO.h"
#include"include/VBO.h"
#include"include/EBO.h"
#include"include/stb/stb_image.h"
#include"include/Texture.h"
#include"include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/Camera.h"
#include "include/Model.h"
#include "include/waveModel.h"

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
	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string texPath = "/glad/resources/";

	//waveModel waveModel;

	
	// Initialize GLFW
	glfwInit();
	
	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "HELLO", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);

	// Generates Shader object using shaders defualt.vert and default.frag
	ObjectShader shaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");

	int n = 256;

	waveModelGPU waveGPU(n);

	// Generates Vertex Array Object and binds it
	//VAO VAO1;
	//VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	//VBO VBO1(&waveGPU.geometry[0], waveGPU.geometry.size() * sizeof(GLfloat));

	// Generates Element Buffer Object and links it to indices
	EBO EBO1(&waveGPU.index[0], waveGPU.index.size() * sizeof(GLuint));
	
	int offset = 3 * std::pow(n, 2);

	// Links VBO to VAO
	//VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 0, (void*)0);
	//VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 0, (void*)(offset * sizeof(float)));
	//VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 0, (void*)(model.textureOffset * sizeof(float)));
	
	// Unbind all to prevent accidentally modifying them
	//VAO1.Unbind();
	//VBO1.Unbind();
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

	glm::vec3 wavePos = glm::vec3(0.0f, 2.5246694087982178f, 0.0f);
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

	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, waveModel.specBuffer);

	//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO1.ID);

	glEnable(GL_DEPTH_TEST);

	Camera camera(800, 800, glm::vec3(500.0f, 500.0f, 700.0f));
    glfwSetWindowUserPointer(window, &camera);

    // Set the key callback function
    glfwSetKeyCallback(window, Camera::staticInputs);
	// Main while loop

	/*
	int work_grp_cnt[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
	std::cout << "Max work groups per compute shader" << 
		" x:" << work_grp_cnt[0] <<
		" y:" << work_grp_cnt[1] <<
		" z:" << work_grp_cnt[2] << "\n";

	int work_grp_size[3];
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	std::cout << "Max work group sizes" <<
		" x:" << work_grp_size[0] <<
		" y:" << work_grp_size[1] <<
		" z:" << work_grp_size[2] << "\n";

	int work_grp_inv;
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	std::cout << "Max invocations count per work group: " << work_grp_inv << "\n";
	*/

	float dt = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		//waveGPU.updateModel(dt);
		dt += 1.0f / 60.0f;
		//dt += 1.0f / 60.0f;
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use

		shaderProgram.Activate();

		//glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO1.ID);

		camera.updateMatrix(45.0f, 0.1f, 10000.0f);

		camera.Matrix(shaderProgram, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		//VAO1.Bind();

		glDrawElements(GL_TRIANGLES, waveGPU.index.size() * sizeof(GLuint) / sizeof(int), GL_UNSIGNED_INT, 0);
		//waveModel.wavePropagation(VAO1.ID, dt);
		// Draw primpopCat.Delete();ers(window);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	//VAO1.Delete();
	//VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
