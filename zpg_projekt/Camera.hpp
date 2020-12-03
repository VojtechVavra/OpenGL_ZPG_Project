#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>

#include <GL/glew.h>	// must be always included first!
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <GLFW/glfw3.h>	// this will include <GL/gl.h>

#include "Subject.hpp"
#include "Object.hpp"

//class Shader;
class Object;

enum movDir {
	//NO_MOVE = 0,
	FORWARD = 1,
	BACKWARD = 2,
	STRAFE_LEFT = 4,
	STRAFE_RIGHT = 8
};

namespace cameraDefaultValues {
	// Default camera values
	const float YAW = 0.0f;			// right / left
	const float PITCH = 0.0f;		// up / down
	const float SPEED = 2.0f;		// camera movement speed
	const float SENSITIVITY = 0.1f;	// mouse sensitivity
	const float FOV = 45.0f;		// camera ZOOM / FOV
}

enum camChange {
	MOVE_ROTATE,
	PROJECTION,
	FLASHLIGHT
};

struct Resolution;

class Camera : public Object, public Subject
{
//struct Resolution;

public:
	struct Resolution {
		int x, y;
	};
	Camera(glm::vec3 position);
	Camera();
	~Camera();
	glm::mat4 getCamera();
	glm::mat4 getProjectionMatrix();
	void setPerspectiveCamera();
	void setPerspectiveCamera(GLfloat width, GLfloat height);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void setMoveDir(movDir _movDir);
	void unSetMoveDir(movDir _movDir);
	void processKeyboard(float deltaTime);
	void processMouseScroll(float yoffset);
	//void update2(Subject* subject) override;

	Resolution getResolution();
	void setResolution(Resolution resolution);

	
	void updateHoldObject(float velocity);
	//void rotateHoldObjectToCamera(float x, float y);
	void setHoldObject(std::shared_ptr<Object> object);
	void dropObject();
	std::shared_ptr<Object> holdObject;
	// end
	void flashLightOnOff();
	bool isFlashLightOn();
	glm::mat4 projection;
	glm::vec3 target;

	float yaw2;
	float pitch2;
private:
	//glm::mat4 projection;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	//glm::vec3 target;

	GLfloat width, height;

	// Euler Angles
	float yaw;
	float pitch;

	float cameraSpeed;
	float sensitivity;
	GLbyte movingDirection;
	float fov;
	bool firstMouse;
	
	bool flashLight;


	void updateCameraVectors();
};

#endif
