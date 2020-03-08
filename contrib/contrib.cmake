if (WIN32)
  #glm
  add_library(glm INTERFACE)
  target_include_directories(glm INTERFACE ${CMAKE_CURRENT_LIST_DIR}/glm/include)

  #glfw3
  add_library(glfw STATIC IMPORTED)
  set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/glfw3/lib/glfw3.lib)
  target_include_directories(glfw INTERFACE ${CMAKE_CURRENT_LIST_DIR}/glfw3/include)

  #glew
  add_library(glew STATIC IMPORTED)
  set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/glew/lib/glew32s.lib)
  target_include_directories(glew INTERFACE ${CMAKE_CURRENT_LIST_DIR}/glew/include)

  #libnoise
  add_library(libnoise STATIC IMPORTED)
  set_target_properties(libnoise PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libnoise/lib/LibNoise64.lib)
  #set_target_properties(libnoise PROPERTIES IMPORTED_IMPLIB ${CMAKE_CURRENT_LIST_DIR}/libnoise/lib/libnoise.lib)
  target_include_directories(libnoise INTERFACE ${CMAKE_CURRENT_LIST_DIR}/libnoise/include)

  #noiseutils
  add_library(noiseutils STATIC IMPORTED)
  set_target_properties(noiseutils PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libnoiseutils/lib/noiseutils.lib)
  target_include_directories(noiseutils INTERFACE ${CMAKE_CURRENT_LIST_DIR}/libnoiseutils/include)
else()  
  find_package(glfw3 REQUIRED)
  find_package(glm REQUIRED)
  find_package(GLEW REQUIRED)

  list(APPEND CMAKE_MODULE_PATH "/home/christoph/dev/libnoise/cmake/Modules/")
  find_package(LibNoise REQUIRED)
endif()

find_package(OpenGL REQUIRED)