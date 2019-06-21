cmake_minimum_required(VERSION 2.8.11)

include(CheckCXXCompilerFlag)

project(shooter_example0)

add_executable(shooter0 main.cpp asteroid.cpp bullet.cpp collision_manager.cpp game.cpp player.cpp)
set_target_properties(shooter0 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(shooter0 sgfx)

if(CLANG_TIDY_EXE)
	set_target_properties(shooter0 PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

# add_subdirectory(../../sgfx build)
# target_link_libraries(shooter0 sgfx glfw GLEW::GLEW OpenGL::GL)

file(COPY img DESTINATION ${CMAKE_BINARY_DIR})

# install(TARGETS shooter0 DESTINATION bin)
