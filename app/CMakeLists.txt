file(GLOB_RECURSE src
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(rpg ${src})

target_link_libraries(rpg glfw GLEW::GLEW ${OPENGL_gl_LIBRARY} glm Threads::Threads ${LIBNOISE_LIBRARY} ${LIBNOISEUTILS_LIBRARY})