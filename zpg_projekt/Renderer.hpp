#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>
#include "Scene.hpp"

class Renderer
{
public:
	Renderer();
	~Renderer();
	void renderScene(std::shared_ptr<Scene> scene, std::shared_ptr<GLFWwindow> window);
private:
	void renderInit();
	void renderLoop();
	void render(Object* object);
	std::shared_ptr<Scene> scene;
	std::shared_ptr<GLFWwindow> window;

	// timing
	float currentFrame;
	float deltaTime;	// time between current frame and last frame
	float lastFrame;

	bool first;
};

#endif // !RENDERER_HPP