cmake_minimum_required(VERSION 2.8.11)

include(CheckCXXCompilerFlag)

project(sat_example)

add_executable(sat_example main.cpp)
set_target_properties(sat_example PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(sat_example sgfx)

if(CLANG_TIDY_EXE)
	set_target_properties(sat_example PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

