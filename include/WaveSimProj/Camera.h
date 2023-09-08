#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"

class Camera
{
    public:
        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 cameraMatrix = glm::mat4(1.0f);

        int width;
        int height;

        float speed = 5.0f;
        float sensitivity = 100.0f;

        Camera(int width, int height, glm::vec3 position);

        void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
        void Matrix(Shader& shader, const char* uniform);

        void Inputs(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void staticInputs(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif