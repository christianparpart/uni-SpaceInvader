cmake_minimum_required(VERSION 2.8.11)

include(CheckCXXCompilerFlag)

project(snake_example)

set(CMAKE_CXX_STANDARD 17)

add_executable(shooter_example main.cpp asteroid.cpp bullet.cpp collision_manager.cpp game.cpp player.cpp)
set_target_properties(shooter_example PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(shooter_example sgfx)

# add_subdirectory(../../sgfx build)
# target_link_libraries(shooter_example sgfx glfw GLEW::GLEW OpenGL::GL)

file(COPY img DESTINATION ${CMAKE_BINARY_DIR})

# install(TARGETS shooter_example DESTINATION bin)
