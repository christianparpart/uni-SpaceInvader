cmake_minimum_required(VERSION 3.0)
project(SpaceInvader CXX)

cmake_policy(SET CMP0028 NEW)

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
	string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
	if(flag_already_set EQUAL -1)
		check_cxx_compiler_flag("${flag}" flag_supported)
		if(flag_supported)
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
		endif()
		unset(flag_supported CACHE)
	endif()
endfunction()

if(MSVC)
	set(TIDY_DEFAULT OFF)
else()
	set(TIDY_DEFAULT ON)
endif()

option(ENABLE_TIDY "Enable clang-tidy [default: OFF]" ${TIDY_DEFAULT})
if(ENABLE_TIDY)
    find_program(CLANG_TIDY_EXE
        NAMES clang-tidy-8 clang-tidy-7 clang-tidy-6.0 clang-tidy
        DOC "Path to clang-tidy executable")
    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found.")
    else()
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
endif()

# i don't like this being enabled *globally*
if(NOT MSVC)
	enable_cxx_compiler_flag_if_supported("-Wall")
endif()
enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-Werror")

add_subdirectory(sgfx)
add_subdirectory(examples/shooter0)
add_subdirectory(examples/shooter1)
