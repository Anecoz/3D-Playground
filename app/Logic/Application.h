#pragma once

struct GLFWwindow;

class Application {
public:
	Application();
  virtual ~Application();

	void run();

  virtual void render() = 0;
  virtual void update(double delta) = 0;

protected:
  GLFWwindow* _window;

private:
	void initWindowHandle();
  void initImgui();

  bool _running;
};

