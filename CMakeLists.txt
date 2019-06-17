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

# bad. should be done per target.
set(CMAKE_CXX_STANDARD 17)

# i don't like this being enabled *globally*
enable_cxx_compiler_flag_if_supported("-Wall")
enable_cxx_compiler_flag_if_supported("-pedantic")
enable_cxx_compiler_flag_if_supported("-Werror")

add_subdirectory(sgfx)
add_subdirectory(examples/shooter0)
