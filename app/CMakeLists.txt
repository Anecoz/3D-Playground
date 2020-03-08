file(GLOB_RECURSE src
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(rpg ${src})

if (WIN32)
  add_definitions(-DGLEW_STATIC)
  target_link_libraries(rpg glfw glew ${OPENGL_gl_LIBRARY} glm libnoise noiseutils)
else()
  set(THREADS_PREFER_PTHREAD_FLAG ON)
  find_package(Threads REQUIRED)
  target_link_libraries(rpg glfw GLEW::GLEW ${OPENGL_gl_LIBRARY} glm Threads::Threads ${LIBNOISE_LIBRARY} ${LIBNOISEUTILS_LIBRARY})
endif()