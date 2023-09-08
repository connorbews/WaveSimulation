#ifndef OPENGLSETUP_CLASS_H
#define OPENGLSETUP_CLASS_H

#include "../../glad/glad.h"
#include <glm/glm.hpp>
#include <iostream>

class OpenGLSetup
{
public:
    OpenGLSetup();

    void ProcessErrors();
    void SetBackgroundColour(float x, float y, float z, float w);
    void CleanBuffer();
    void GetMaxWorkGroupSize();
    void GetMaxInvocations();
};

#endif