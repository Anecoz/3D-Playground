find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)

list(APPEND CMAKE_MODULE_PATH "/home/christoph/dev/libnoise/cmake/Modules/")
find_package(LibNoise REQUIRED)