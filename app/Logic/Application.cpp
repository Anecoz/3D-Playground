#include "Application.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../Input/KeyInput.h"
#include "../Input/MousePosInput.h"
#include "../Input/MouseButtonInput.h"

#include <iostream>
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

  // Dear, imgui
  initImgui();
}

Application::~Application()
{
  glfwDestroyWindow(_window);
  glfwTerminate();
}

void Application::run()
{
  // Init Main loop stuff
  int frames = 0;
  std::string fps = " fps";

  _running = true;

	// Main loop
  double lastUpdate = glfwGetTime();
  double lastFrameUpdate = glfwGetTime();
	while (_running  && !glfwWindowShouldClose(_window)) {
    // Logic update
    glfwPollEvents();

    // ImGui stuff
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Test IMGUI
    {
      //static float f = 0.0f;
      //static int counter = 0;

      ImGui::Begin("FPS counter");                          // Create a window called "Hello, world!" and append into it.

      //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
      //ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
      //ImGui::Checkbox("Another Window", &show_another_window);

      //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      /*if (ImGui::Button("Button")) {                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
      }*/
      //ImGui::SameLine();
      //ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    double now = glfwGetTime();
		double delta = (now - lastUpdate);
    lastUpdate = now;
    update(delta);

		// Render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();

    // ImGui render
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	  glfwSwapBuffers(_window);
    // Render stop

		frames++;
    double now2 = glfwGetTime();
		if ((now2 - lastFrameUpdate) > 1.0) {
      lastFrameUpdate = now2;
			glfwSetWindowTitle(_window, std::string("Disim " + std::to_string(frames) + fps).c_str());
			frames = 0;
		}
	}
  // Do cleanup here
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void Application::initWindowHandle()
{
	//glfwWindowHint(GLFW_SAMPLES, 8);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(1920, 1080, "Disim", NULL, NULL);

	if (!_window) {
		std::cerr << "Failed to open GLFW window" << std::endl;
		glfwTerminate();
		exit(-2);
	}

	glfwMakeContextCurrent(_window);
	glfwSwapInterval(0); // Vsync or not

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

	glClearColor((GLclampf)(5.0/255.0), (GLclampf)(209.0/255.0), (GLclampf)(255.0/255.0), (GLclampf)0.0);
	glEnable(GL_DEPTH_TEST);
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Supported OpenGL version: " << glGetString(GL_VERSION) << std::endl;
  GLint maxPatchVertices = 0;
  glGetIntegerv(GL_MAX_PATCH_VERTICES, &maxPatchVertices);
  std::cout << "Max supported patch vertices: " << std::to_string(maxPatchVertices) << std::endl;
  glPatchParameteri(GL_PATCH_VERTICES, 3);
  glEnable(GL_CULL_FACE);
}

void Application::initImgui()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(_window, true);
  ImGui_ImplOpenGL3_Init();
}