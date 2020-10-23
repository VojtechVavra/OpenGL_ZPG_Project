#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp> 
#include "Shader.hpp"

class Shader;

class Camera
{
public:
    Camera();
    Camera(Shader* shader);
    void cameraDirection(float fi, float psi);  // return target
    glm::mat4 getCamera();          // return: lookAt (glm::vec3 eye, glm::vec3 target, glm::vec3 up)
    //glm::mat4 GetViewProjection();  // added
    void setPerspectiveCamera();
    void moveForward();      // added
    void moveBackward();     // added
    void moveLeft();
    void moveRight();

    void LookAtTarget(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);
    glm::vec3 _direction;

    // euler Angles
    float yaw;
    float pitch;
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 worldUp;

    // walk
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);   // 0, 1, 0
    glm::mat4 GetViewMatrix();
    void updateCameraVectors();
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    glm::vec3 target;       // camera direction
private:
    Shader* m_shader;
    //glm::vec3 target;       // camera direction
    glm::vec3 eyePosition;  // eye
    glm::vec3 up;

    glm::mat4 projection;   // perspective
    float movementSpeed = 0.1;
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right;
    float cameraSpeed = 0.1f;
    // 
    int lastMouseX, lastMouseY;
    float leftRight; // right left
    float upDown;   // up down
    //float MAX_PITCH = 89.999f;
    //float MIN_PITCH = -MAX_PITCH;
};





#endif