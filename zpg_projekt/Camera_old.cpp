#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera_old.hpp"
#include "Shader.hpp"

Camera::Camera() {

}

Camera::Camera(Shader* shader)
{
    m_shader = shader;
    eyePosition = glm::vec3(-0.4f, 0.5f, -1.0f);  // position of the camera's viewpoint. Originally eye
    //target = glm::vec3(-0.4f, 0.5f, -1.0f);       // target where we look
    target = glm::vec3(0.0f, 0.0f, 0.0f);       // target where we look
    up = glm::vec3(0.0f, 1.0f, 0.0f);   // almost always it will be vector(0, 1, 0).
                            // The up vector which makes the camera fixed and cannot be rotate.

    this->position = glm::vec3(-0.4f, 0.5f, -1.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->worldUp = up;
    this->yaw = -90.0f;
    this->pitch = 0.0f;
    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    //updateCameraVectors();
}

glm::mat4 Camera::getCamera()
{
    return glm::lookAt(eyePosition, eyePosition + target, up);
}

void Camera::cameraDirection(float fi, float psi)
{
    // výpoèet smìru kamery 
    //target.x = cos(fi) * 0.05;
    //target.z = sin(fi) * 0.05;
    //target.y = sin(psi) * 0.05;

    target.x = cos(glm::radians(fi)) * cos(glm::radians(psi)) * 0.05;
    target.y = sin(glm::radians(psi)) * 0.05;
    target.z = sin(glm::radians(fi)) * sin(glm::radians(psi)) * 0.05;
    target = glm::normalize(target);

    right = glm::normalize(glm::cross(this->target, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, this->target));
}

void Camera::setPerspectiveCamera()
{
    // Nastaveni projekèní matice na perspektivní promítání
    // Projection matrix:            45° Field of View, 4:3 ratio, display range: 0.1 unit, 100 units
    // perspective
    glm::mat4 projection = glm::perspective(glm::radians(80.0f), 4.0f / 3.0f, 0.01f, 100.0f);
    //perspective = projection;

    m_shader->sendUniform("projectionMatrix", projection);
}

void Camera::moveForward()
{
    cameraPos += cameraSpeed * glm::vec3(target.x, 0, target.z);
}

void Camera::moveBackward()
{
    cameraPos -= cameraSpeed * glm::vec3(target.x, 0, target.z);
}

void Camera::moveLeft()
{
    cameraPos -= glm::normalize(glm::cross(target, up)) * cameraSpeed;
}

void Camera::moveRight()
{
    cameraPos += glm::normalize(glm::cross(target, up)) * cameraSpeed;
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    //return glm::lookAt(position, position + front, up);
    //return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    return glm::lookAt(cameraPos, cameraPos + target, up);
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
/*void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    float mouseSensitivity = 0.5f;
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}
*/

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
}