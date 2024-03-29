cmake_minimum_required(VERSION 2.8.11)

include(CheckCXXCompilerFlag)

project(libsgfx)

find_package(glfw3)
find_package(GLEW)
find_package(OpenGL)

add_library(sgfx STATIC src/canvas.cpp src/image.cpp src/primitives.cpp src/window.cpp)
set_target_properties(sgfx PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_include_directories(sgfx PUBLIC include)
target_link_libraries(sgfx glfw GLEW::GLEW OpenGL::GL)

if(CLANG_TIDY_EXE)
	set_target_properties(sgfx PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

# install(TARGETS sgfx DESTINATION lib)
# install(DIRECTORY include/sgfx DESTINATION include)
