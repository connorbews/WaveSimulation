#ifndef GLFWSETUP_CLASS_H
#define GLFWSETUP_CLASS_H

#include <iostream>
#include "Camera.h"

class GLFWSetup
{
public:
    GLFWSetup(Camera* camera);

    bool ShouldWindowClose();
    void UpdateScreenInputs();

    ~GLFWSetup();
private:
    GLFWwindow* window;
    Camera* camera;
};

#endif