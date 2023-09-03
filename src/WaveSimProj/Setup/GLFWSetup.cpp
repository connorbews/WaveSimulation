#include "../../../include/WaveSimProj/Setup/GLFWSetup.h"

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

bool GLFWSetup::ShouldWindowClose()
{
    return !glfwWindowShouldClose(window);
}

void GLFWSetup::UpdateScreenInputs()
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