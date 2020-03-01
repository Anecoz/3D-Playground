#include "Application.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../Input/KeyInput.h"
#include "../Input/MousePosInput.h"
#include "../Input/MouseButtonInput.h"

#include <iostream>
#include <chrono>
#include <string>

static void errorCallback(int error, const char* description)
{
	std::cerr << "GLFW Error code: " << std::to_string(error) << ", description: " << description << std::endl;
}

Application::Application() :
  _running(false)
{
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
		exit(-1);

  // Create gl window stuff
  initWindowHandle();
}

Application::~Application()
{
  glfwDestroyWindow(_window);
}

void Application::run()
{
	// Init Main loop stuff
	using namespace std::chrono;
  auto ns = 1000000000.0 / 60.0;
  auto timer = glfwGetTime();
	//int updates = 0;
	int frames = 0;
	std::string ups = " ups ";
  std::string fps = " fps";

  _running = true;

	// Main loop
  double lastUpdate = glfwGetTime();
  double lastFrameUpdate = glfwGetTime();
	while (_running  && !glfwWindowShouldClose(_window)) {
    if (KeyInput::isKeyClicked(GLFW_KEY_ESCAPE)) break;

    // Logic update
    glfwPollEvents();

    double now = glfwGetTime();
		double delta = (now - lastUpdate);
    lastUpdate = now;
    update(delta);

		// Logic Update
		/*if (delta >= 1.0) {
			glfwPollEvents();
			updates++;
			delta--;

      update(delta);
      lastTime = now;
		}*/

		// Render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();
		glfwSwapBuffers(_window);
		// Render stop

		frames++;
    double now2 = glfwGetTime();
		if ((now2 - lastFrameUpdate) > 1.0) {
      lastFrameUpdate = now2;
			//timer += 1000;
			glfwSetWindowTitle(_window, std::string("Disim " + std::to_string(frames) + fps).c_str());
			//updates = 0;
			frames = 0;
		}
	}
  // Do cleanup here
}

void Application::initWindowHandle()
{
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(1280, 720, "Disim", NULL, NULL);

	if (!_window) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		exit(-2);
	}

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1); // Vsync or not

	// Set input callbacks
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(_window, KeyInput::invoke);
	//glfwSetCursorPosCallback(_window, MousePosInput::invoke);
  MousePosInput::_window = _window;
	glfwSetMouseButtonCallback(_window, MouseButtonInput::invoke);

	// Check for errors
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-3);
	}

	glClearColor(0.7, 0.7, 0.7, 0.0);
	glEnable(GL_DEPTH_TEST);
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Supported OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  GLint maxPatchVertices = 0;
  glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
  std::cout << "Max supported patch vertices: " << std::to_string(maxPatchVertices) << std::endl;
  glPatchParameteri(GL_PATCH_VERTICES, 3);
  glDisable(GL_CULL_FACE);
}