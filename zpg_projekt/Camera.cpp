#include <string>

#include <glm/gtc/matrix_transform.hpp>	// glm::lookAt

#include "Camera.hpp"
#include "Shader.hpp"
#include "Object.hpp"

// https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.4.camera_class/camera_class.cpp

/*struct Camera::Resolution {
	int x, y;
};*/

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

	this->width = 800;
	this->height = 600;

	this->flashLight = false;

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
{	// view projection
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
	projection = glm::perspective(glm::radians(fov), width / height, 0.01f, 100.0f);

	notifyObservers(this, camChange::PROJECTION);	// zoomed
	//notifyObservers(this);	// zoomed
}

void Camera::setPerspectiveCamera(GLfloat width, GLfloat height)
{
	// Nastaveni projekèní matice na perspektivní promítání	(perspective projection)
	// Projection matrix:            45° Field of View, 4:3 ratio, display range: 0.1 unit, 100 units
	this->width = width;
	this->height = height;
	GLfloat aspectRatio = width / height;	// 4/3 default 800x600

	this->projection = glm::perspective(glm::radians(fov), aspectRatio, 0.01f, 100.0f);

	notifyObservers(this, camChange::PROJECTION);	// zoomed
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;	

	yaw2 = yaw;
	pitch2 = pitch;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	if (holdObject != nullptr)
	{
		//added
		//rotateHoldObjectToCamera(xoffset, yoffset);
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

	if (holdObject != nullptr)
	{
		//added
		//rotateHoldObjectToCamera(this->target.x, this->target.y);
	}

	notifyObservers(this, camChange::MOVE_ROTATE);
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

	if (holdObject != nullptr)
	{
		//added
		updateHoldObject(velocity);	// posun drziciho objektu
	}

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
	else
	{
		return;
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

Camera::Resolution Camera::getResolution()
{
	//Resolution r{ 10,10 };
	return Resolution{ static_cast<int>(width), static_cast<int>(height) };
	//return Resolution n{ &static_cast<int>(width), &static_cast<int>(height) };
}

void Camera::setResolution(Resolution resolution)
{
	this->width = resolution.x;
	this->height = resolution.y;
}

void Camera::updateHoldObject(float velocity)
{
	//TODO: spocitat vektor od kamery k objketu
	// pozice kamery + vektor

	glm::vec3 shiftPosition(0.0f, 0.0f, 0.0f);
	float calibFactor = 5.0f;	// TODO: calibFactor zavisi na velikosti (scale meritku) objektu,
	// proto se kazdy objekt pohybuje jinak rychle, podle toho, jaky ma scale
	bool move = false;

	if (movingDirection == (FORWARD | STRAFE_RIGHT)) {
		move = true;
		shiftPosition = calibFactor * velocity * target * 0.5f;
		shiftPosition += glm::normalize(glm::cross(target, up)) * calibFactor * velocity * 0.5f;
	}
	else if (movingDirection == (FORWARD | STRAFE_LEFT)) {
		move = true;
		shiftPosition = calibFactor * velocity * target * 0.5f;
		shiftPosition -= (glm::normalize(glm::cross(target, up)) * calibFactor * velocity * 0.5f);
	}
	else if (movingDirection == (BACKWARD | STRAFE_RIGHT)) {
		move = true;
		shiftPosition = -(calibFactor * velocity * target * 0.5f);
		shiftPosition += glm::normalize(glm::cross(target, up)) * calibFactor * velocity * 0.5f;
	}
	else if (movingDirection == (BACKWARD | STRAFE_LEFT)) {
		move = true;
		shiftPosition = -(calibFactor * velocity * target * 0.5f);
		shiftPosition -= (glm::normalize(glm::cross(target, up)) * calibFactor * velocity * 0.5f);
	}

	else if (movingDirection & FORWARD) {		// 'w'
		move = true;
		shiftPosition = calibFactor * velocity * target;	// 'target' = 'front'
	}
	else if (movingDirection & BACKWARD) {		// 's'
		move = true;
		shiftPosition = -(calibFactor * velocity * target);
	}
	else if (movingDirection & STRAFE_LEFT) {	// 'a'
		move = true;
		shiftPosition = -(glm::normalize(glm::cross(target, up)) * velocity * calibFactor);
	}
	else if (movingDirection & STRAFE_RIGHT) {	// 'd'
		move = true;
		shiftPosition = glm::normalize(glm::cross(target, up)) * velocity * calibFactor;
	}


	if (move)
	{
		holdObject->Translate(shiftPosition);

		printf("Object shifted by: %f, %f, %f\n", shiftPosition.x, shiftPosition.y, shiftPosition.z);

		holdObject->useShader();
		Shader::sendUniform(holdObject->getShader(), "modelMatrix", holdObject->getMatrix());
		Shader::sendUniform(holdObject->getShader(), "fragmentColor", holdObject->getColor());
	}
}


void Camera::setHoldObject(std::shared_ptr<Object> object)
{
	this->holdObject = object;
}

void Camera::dropObject()
{
	this->holdObject = nullptr;
}

void Camera::flashLightOnOff()
{
	this->flashLight = this->flashLight ? false : true;
	notifyObservers(this, camChange::FLASHLIGHT);
	printf("Flashlight %s\n", flashLight ? "on" : "off");
}

bool Camera::isFlashLightOn()
{
	return this->flashLight;
}

/*void update2(Subject* subject)
{}*/