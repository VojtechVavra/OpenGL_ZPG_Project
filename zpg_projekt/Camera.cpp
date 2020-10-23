#include <string>
#include <glm/gtc/matrix_transform.hpp>	// glm::lookAt
#include "Camera.hpp"
#include "Shader.hpp"

// https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.4.camera_class/camera_class.cpp

Camera::Camera(glm::vec3 position) : Object(position)
{
	this->movingDirection = 0;
	this->target = glm::vec3(0.0f, 0.0f, 0.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	this->firstMouse = true;
	this->yaw = cameraDefaultValues::YAW;
	this->pitch = cameraDefaultValues::PITCH;
	this->fov = cameraDefaultValues::FOV;
	this->cameraSpeed = cameraDefaultValues::SPEED;
	this->sensitivity = cameraDefaultValues::SENSITIVITY;

	setPerspectiveCamera();
	updateCameraVectors();
}

Camera::Camera() //: Object()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getCamera()
{
	return glm::lookAt(position, position + target, up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return this->projection;
}

void Camera::setPerspectiveCamera()
{
	// Nastaveni projekèní matice na perspektivní promítání	(perspective projection)
	// Projection matrix:            45° Field of View, 4:3 ratio, display range: 0.1 unit, 100 units
	projection = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.01f, 100.0f);
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
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

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 newTarget;	// 'target' = 'front'
	newTarget.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newTarget.y = sin(glm::radians(pitch));
	newTarget.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->target = glm::normalize(newTarget);

	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(this->target, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, this->target));	// 'target' = 'front'
}

void Camera::setMoveDir(movDir movDirection)
{
	this->movingDirection |= movDirection;
}

void Camera::unSetMoveDir(movDir movDirection)
{
	this->movingDirection &= ~movDirection;
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::processKeyboard(float deltaTime)
{
	float velocity = cameraSpeed * deltaTime;

	if (movingDirection == (FORWARD | STRAFE_RIGHT)) {
		position += velocity * target * 0.5f;
		position += glm::normalize(glm::cross(target, up)) * velocity * 0.5f;
	}
	else if (movingDirection == (FORWARD | STRAFE_LEFT)) {
		position += velocity * target * 0.5f;
		position -= glm::normalize(glm::cross(target, up)) * velocity * 0.5f;
	}
	else if (movingDirection == (BACKWARD | STRAFE_RIGHT)) {
		position -= velocity * target * 0.5f;
		position += glm::normalize(glm::cross(target, up)) * velocity * 0.5f;
	}
	else if (movingDirection == (BACKWARD | STRAFE_LEFT)) {
		position -= velocity * target * 0.5f;
		position -= glm::normalize(glm::cross(target, up)) * velocity * 0.5f;
	}
	else if (movingDirection & FORWARD) {		// 'w'
		
		position += velocity * target;	// 'target' = 'front'
		//position += velocity * glm::vec3(target.x, 0, target.z);   // this is useful for staying on ground -> constant Y axis
		//position += target * velocity;
	}
	else if (movingDirection & BACKWARD) {		// 's'
		position -= velocity * target;
		//position -= velocity * glm::vec3(target.x, 0, target.z);	// this is useful for staying on ground -> constant Y axis
		//position -= target * velocity;
	}
	else if (movingDirection & STRAFE_LEFT) {	// 'a'
		position -= glm::normalize(glm::cross(target, up)) * velocity;
		//position -= right * velocity;
	}
	else if (movingDirection & STRAFE_RIGHT) {	// 'd'
		position += glm::normalize(glm::cross(target, up)) * velocity;
		//position += right * velocity;
	}
	
	// make sure the user stays at the ground level
	//position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)

	this->updateCameraVectors();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::processMouseScroll(float yoffset)
{
	// camera zoom
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
	setPerspectiveCamera();

	printf("Zoomed(fov): %d\n", static_cast<int>(fov));
}
