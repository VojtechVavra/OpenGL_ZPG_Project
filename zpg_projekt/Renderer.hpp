#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

#include "Scene.hpp"
#include "Flame.hpp"

class Renderer
{
public:
	Renderer();
	~Renderer();
	void renderScene(std::shared_ptr<Scene> scene, GLFWwindow* window);
private:
	void renderInit();
	void renderLoop();
	//void renderObject(Object* object);
	void renderObject(std::shared_ptr<Object> object);
	std::shared_ptr<Scene> scene;
	//std::shared_ptr<GLFWwindow> window;
	GLFWwindow* window;
	void renderModel(int i_stencil_offset);
	void renderModel2(int meshModel); // 2021 - repairing loading models

	void printMemoryUsage(std::string number);
	Flame* flame;
	// timing
	float currentFrame;
	float deltaTime;	// time between current frame and last frame
	float lastFrame;
};

#endif // !RENDERER_HPP