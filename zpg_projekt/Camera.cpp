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

	this->flashLightBool = false;
	this->showTextureDetail = false;
	this->fly = false;

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
	return m_lookAt;
	// return glm::lookAt(position, position + target, up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return this->projection;
}

void Camera::setPerspectiveCamera()
{
	// Nastaveni projek�n� matice na perspektivn� prom�t�n�	(perspective projection)
	// Projection matrix:            45� Field of View, 4:3 ratio, display range: 0.1 unit, 100 units
	projection = glm::perspective(glm::radians(fov), width / height, 0.01f, 100.0f);

	notifyObservers(this, camChange::PROJECTION);	// zoomed
	//notifyObservers(this);	// zoomed
}

void Camera::setPerspectiveCamera(GLfloat width, GLfloat height)
{
	// Nastaveni projek�n� matice na perspektivn� prom�t�n�	(perspective projection)
	// Projection matrix:            45� Field of View, 4:3 ratio, display range: 0.1 unit, 100 units
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
		//Shader::use(holdObject->getShader());
		//Shader::sendUniform(holdObject->getShader(), "modelMatrix", position + glm::normalize(target) * 5.0f);
		/*glm::mat4 newMatrix = glm::mat4(1.0f);
		newMatrix = glm::rotate(newMatrix, target.x + holdObject->getRotate().x, glm::vec3(0.0f, 1.0f, 0.0f));
		newMatrix = glm::rotate(newMatrix, target.y + holdObject->getRotate().y, glm::vec3(0.0f, 1.0f, 0.0f));
		newMatrix = glm::rotate(newMatrix, target.z + holdObject->getRotate().z, glm::vec3(0.0f, 0.0f, 1.0f));
		newMatrix = glm::translate(newMatrix, position + glm::normalize(target) * 5.0f);
		Shader::sendUniform(holdObject->getShader(), "modelMatrix", newMatrix);*/
	}

	m_lookAt = glm::lookAt(position, position + target, up);

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
		//updateHoldObject(velocity);	// posun drziciho objektu
		Shader::use(holdObject->getShaderID());
		Shader::sendUniform(holdObject->getShaderID(), "modelMatrix", position + glm::normalize(target) * 5.0f);
	}

	if (movingDirection == (FORWARD | STRAFE_RIGHT)) {
		if (fly) {
			position += velocity * target * 0.5f;
			position += glm::normalize(glm::cross(target, up)) * velocity * 0.5f;
		}
		else {
			position += velocity * glm::vec3(target.x, 0, target.z) * 0.5f;
			position += glm::normalize(glm::cross(glm::vec3(target.x, 0, target.z), up)) * velocity * 0.5f;
		}
	}
	else if (movingDirection == (FORWARD | STRAFE_LEFT)) {
		if (fly) {
			position += velocity * target * 0.5f;
			position -= glm::normalize(glm::cross(target, up)) * velocity * 0.5f;
		}
		else {
			position += velocity * glm::vec3(target.x, 0, target.z) * 0.5f;
			position -= glm::normalize(glm::cross(glm::vec3(target.x, 0, target.z), up)) * velocity * 0.5f;
		}
	}
	else if (movingDirection == (BACKWARD | STRAFE_RIGHT)) {
		if (fly) {
			position -= velocity * target * 0.5f;
			position += glm::normalize(glm::cross(target, up)) * velocity * 0.5f;
		}
		else {
			position -= velocity * glm::vec3(target.x, 0, target.z) * 0.5f;
			position += glm::normalize(glm::cross(glm::vec3(target.x, 0, target.z), up)) * velocity * 0.5f;
		}		
	}
	else if (movingDirection == (BACKWARD | STRAFE_LEFT)) {
		if (fly) {
			position -= velocity * target * 0.5f;
			position -= glm::normalize(glm::cross(target, up)) * velocity * 0.5f;
		}
		else {
			position -= velocity * glm::vec3(target.x, 0, target.z) * 0.5f;
			position -= glm::normalize(glm::cross(glm::vec3(target.x, 0, target.z), up)) * velocity * 0.5f;
		}	
	}

	else if (movingDirection & FORWARD) {		// 'w'
		if (fly) {
			position += velocity * target;	// 'target' = 'front'
		}
		else {
			position += velocity * glm::vec3(target.x, 0, target.z);
		}
		//position += velocity * glm::vec3(target.x, 0, target.z);   // this is useful for staying on ground -> constant Y axis
		//position += target * velocity;
	}
	else if (movingDirection & BACKWARD) {		// 's'
		if (fly) {
			position -= velocity * target;
		}
		else {
			position -= velocity * glm::vec3(target.x, 0, target.z);
		}
		//position -= velocity * glm::vec3(target.x, 0, target.z);	// this is useful for staying on ground -> constant Y axis
		//position -= target * velocity;
	}
	else if (movingDirection & STRAFE_LEFT) {	// 'a'
		if (fly) {
			position -= glm::normalize(glm::cross(target, up)) * velocity;
		}
		else {
			position -= glm::normalize(glm::cross(glm::vec3(target.x, 0, target.z), up)) * velocity;
		}
		//position -= right * velocity;
	}
	else if (movingDirection & STRAFE_RIGHT) {	// 'd'
		
		if (fly) {
			position += glm::normalize(glm::cross(target, up)) * velocity;
		}
		else
		{
			position += glm::normalize(glm::cross(glm::vec3(target.x, 0, target.z), up)) * velocity;
		}
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

/*void Camera::updateHoldObject(float velocity)
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
		//holdObject->Translate(shiftPosition);	// TODO: oddelat zakomentovani

		printf("Object shifted by: %f, %f, %f\n", shiftPosition.x, shiftPosition.y, shiftPosition.z);

		holdObject->useShader();
		Shader::sendUniform(holdObject->getShader(), "modelMatrix", holdObject->getMatrix());
		Shader::sendUniform(holdObject->getShader(), "fragmentColor", holdObject->getColor());
	}
}*/


void Camera::setHoldObject(std::shared_ptr<Object> object)
{
	this->holdObject = object;
	Shader::use(holdObject->getShaderID());
	Shader::sendUniform(holdObject->getShaderID(), "isHeld", true);
	//Shader::sendUniform(holdObject->getShader(), "modelMatrix", position + glm::normalize(target) + 10.0f);

	glm::mat4 newMatrix = glm::mat4(1.0);
	newMatrix = glm::translate(newMatrix, glm::vec3(5.0, 5.0, 5.0) * target);
	//newMatrix = glm::rotate(newMatrix, holdObject->getRotate().x + target.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//newMatrix = glm::rotate(newMatrix, holdObject->getRotate().z + target.z, glm::vec3(0.0f, 0.0f, 1.0f));
	holdObject->Scale(newMatrix, holdObject->getScale());

	holdObject->setPositionWithoutTranslate(position + target * 15.0f);
	holdObject->setMatrix(newMatrix);
	Shader::sendUniform(holdObject->getShaderID(), "modelMatrix", newMatrix);
	//glm::lookAt(position, position + target, up);
	Shader::use(0);
}

void Camera::dropObject()
{
	//holdObject->setPositionWithoutTranslate(position + glm::normalize(target) * 5.0f);
	//holdObject->setPosition(position + glm::normalize(target) * 5.0f);
	glm::mat4 newMatrix = glm::mat4(1.0);		// ModelMatrix = T*S*R
	//newMatrix = glm::translate(newMatrix, position + target * 5.0f);	// uncoment this

	//newMatrix = glm::scale(newMatrix, holdObject->getScale());

	//newMatrix = glm::rotate(newMatrix, target.x + holdObject->getRotate().x, glm::vec3(0.0f, 1.0f, 0.0f));
	//newMatrix = glm::rotate(newMatrix, target.y + holdObject->getRotate().y, glm::vec3(0.0f, 1.0f, 0.0f));
	//newMatrix = glm::rotate(newMatrix, target.z + holdObject->getRotate().z, glm::vec3(0.0f, 0.0f, 1.0f));
	
	//newMatrix = glm::lookAt(-position, position + target * (5.0f), up);
	newMatrix = glm::translate(newMatrix, position + target * 5.0f);
	newMatrix = glm::scale(newMatrix, holdObject->getScale());
	//newMatrix = glm::rotate(newMatrix, holdObject->getRotate().z + target.z, glm::vec3(0.0f, 0.0f, 1.0f));
	holdObject->setMatrix(newMatrix);
	holdObject->setPositionWithoutTranslate(position + target * 5.0f);
	printf("scale: x>%f y>%f z%f", holdObject->getScale().x, holdObject->getScale().y, holdObject->getScale().z);

	holdObject->Scale(newMatrix, holdObject->getScale());

	Shader::use(holdObject->getShaderID());
	Shader::sendUniform(holdObject->getShaderID(), "isHeld", false);
	Shader::sendUniform(holdObject->getShaderID(), "modelMatrix", holdObject->getMatrix());
	//Shader::sendUniform(holdObject->getShader(), "modelMatrix", position + glm::normalize(target) * 5.0f);

	this->holdObject = nullptr;	
}

void Camera::setHoldObject2(std::shared_ptr<MeshLoader> object)
{
	GLuint shaderProgramID = Shader::getShader(ShaderType::GRAB_MODEL);
	object->shaderProgramID = shaderProgramID;
	glUseProgram(shaderProgramID);
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.25f, -0.26f, -0.6f));
	if (object->fileName == "vetev2.obj" || object->fileName == "vetev1.obj") {
		ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.25f, -0.1f, -0.6f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	}
	else {
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	}
	
	//glm::mat4 ModelMatrix = getCamera();
	//ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	object->ModelMatrix = ModelMatrix;
	//this->holdObjects.push_back(object);
	holdObj = object;
	Shader::sendUniform(shaderProgramID, "modelMatrix", object->ModelMatrix);
	holdingObject = true;
	//Shader::sendUniform(holdObject->getShader(), "modelMatrix", newMatrix);
	//glm::lookAt(position, position + target, up);
	//Shader::use(0);
}

void Camera::dropObject2()
{
	//if (holdingObject) {
		GLuint shaderProgramID = Shader::getShader(ShaderType::DIFFUSE_MODEL);
		holdObj->shaderProgramID = shaderProgramID;
		glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0), getPosition() + target * 1.3f);
		//ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.25f, -0.26f, 0.6f));
		if (holdObj->fileName == "vetev2.obj" || holdObj->fileName == "vetev1.obj") {
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
		}
		else {
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
		}
		holdObj->ModelMatrix = ModelMatrix;
		holdingObject = false;
	//}
	
	//holdObjects[0]->shaderProgramID = shaderProgramID;

	//for (auto& holdObj : holdObjects) {
	/*for(int i = 0; i < holdObjects.size(); i++) {
		holdObjects[i]->shaderProgramID = shaderProgramID;
		holdObjects[i] = nullptr;
	}*/
	//holdObjects.clear();

	/*glm::mat4 newMatrix = glm::mat4(1.0);		// ModelMatrix = T*S*R

	//newMatrix = glm::lookAt(-position, position + target * (5.0f), up);
	newMatrix = glm::translate(newMatrix, position + target * 5.0f);
	newMatrix = glm::scale(newMatrix, holdObject->getScale());
	//newMatrix = glm::rotate(newMatrix, holdObject->getRotate().z + target.z, glm::vec3(0.0f, 0.0f, 1.0f));
	holdObject->setMatrix(newMatrix);
	holdObject->setPositionWithoutTranslate(position + target * 5.0f);
	printf("scale: x>%f y>%f z%f", holdObject->getScale().x, holdObject->getScale().y, holdObject->getScale().z);

	holdObject->Scale(newMatrix, holdObject->getScale());

	Shader::use(holdObject->getShader());
	Shader::sendUniform(holdObject->getShader(), "isHeld", false);
	Shader::sendUniform(holdObject->getShader(), "modelMatrix", holdObject->getMatrix());
	//Shader::sendUniform(holdObject->getShader(), "modelMatrix", position + glm::normalize(target) * 5.0f);
	
	this->holdObject = nullptr;*/
}


void Camera::flashLightOnOff()
{
	this->flashLightBool = this->flashLightBool ? false : true;
	notifyObservers(this, camChange::FLASHLIGHT);
	printf("Flashlight %s\n", flashLightBool ? "on" : "off");
}

void Camera::setFLightState(bool state)
{
	this->flashLightBool = state;
	notifyObservers(this, camChange::FLASHLIGHT);
	//printf("Flashlight %s\n", flashLightBool ? "on" : "off");
}

bool Camera::isFlashLightOn()
{
	return this->flashLightBool;
}

void Camera::setShowTextureDetail(bool show) {
	this->showTextureDetail = show;
	notifyObservers(this, camChange::SHOW_TEXTURE_DETAIL);
}

bool Camera::getTextureDetail() {
	return this->showTextureDetail;
}


/*void update2(Subject* subject)
{}*/