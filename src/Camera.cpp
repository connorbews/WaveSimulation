#include "../include/Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + Orientation, Up);
    proj = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);
    rotate = glm::rotate(proj * view, glm::radians(20.0f), glm::cross(Orientation, Up));

    cameraMatrix = rotate;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W)
    {
        Position += speed * Orientation;
    }
    else if (key == GLFW_KEY_A)
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
    }
    else if (key == GLFW_KEY_S)
    {
        Position += speed * -Orientation;
    }
    else if (key == GLFW_KEY_D)
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up));
    }
    else if (key == GLFW_KEY_SPACE)
    {
        Position += speed * Up;
    }
    else if (key == GLFW_KEY_LEFT_CONTROL)
    {
        Position += speed * -Up;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
    {
        speed = 0.4f;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    {
        speed = 0.1f;
    }
}

void Camera::staticInputs(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Camera* _camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    _camera->Inputs(window, key, scancode, action, mods);
}