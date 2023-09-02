#include "../include/GLFWSetup.h"

GLFWSetup::GLFWSetup(Camera* camera)
{
    if (glfwInit() == GLFW_FALSE)
    {
        std::cerr << "An error has occurred during glfw initialization" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 800, "HELLO", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, camera);
    glfwSetKeyCallback(window, Camera::staticInputs);
}

bool GLFWSetup::GLFWWindowShouldClose()
{
    return !glfwWindowShouldClose(window);
}

void GLFWSetup::GLFWWindowUpdate()
{
    glfwSwapBuffers(window);
	glfwPollEvents();
}

GLFWSetup::~GLFWSetup()
{
    free(camera);
	glfwDestroyWindow(window);
	glfwTerminate();
}